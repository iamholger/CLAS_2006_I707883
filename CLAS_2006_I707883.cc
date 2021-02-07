// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/DressedLeptons.hh"
#include "Rivet/Projections/PromptFinalState.hh"

#include <fstream>


// Bisection would be nice but this is robust and easy to understand
int findBin(double const arr[], double const key, const size_t size)
{

    if (key<arr[0] or key>arr[size-1]) return -1;

    int l = 0;
    for (int i=0;i<size;i++)
    {
        if (arr[i]>key) break;
        l = i;
    }

    return l;
}

double constexpr arrQ2[5]  = {0.25, 0.35, 0.45, 0.55, 0.65};
double constexpr arrW[26]  = {1.10, 1.12, 1.14, 1.16, 1.18, 1.20, 1.22, 1.24, 1.26, 1.28, 1.30, 1.32, 1.34, 1.36, 1.38, 1.40, 1.42, 1.44, 1.46, 1.48, 1.50, 1.52, 1.54, 1.56, 1.58, 1.60};
double constexpr arrTh[13] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180};
double constexpr arrPh[13] = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 360};




//constexpr double widthQ2(0.1);   // GeV*GeV
//const double widthW(20);     // MeV
//const double widthTheta(15); // deg
//const double widthPhi(30);   // deg

namespace Rivet {


  /// @brief Add a short analysis description here
  class CLAS_2006_I707883 : public Analysis {
  public:

    /// Constructor
    RIVET_DEFAULT_ANALYSIS_CTOR(CLAS_2006_I707883);


    /// @name Analysis methods
    ///@{

    /// Book histograms and initialise projections before the run
    void init()
    {

      wtot_=0;
      for (int i=0;i<4;i++)
      for (int j=0;j<25;j++)
      for (int k=0;k<12;k++)
      for (int l=0;l<12;l++) 
      {
        DATA_[i][j][k][l] = 0.0;
      }

      // The basic final-state projection:
      // all final-state particles within
      // the given eta acceptance
      const FinalState fs(Cuts::abseta < 4.9);

      // The final-state particles declared above are clustered using FastJet with
      // the anti-kT algorithm and a jet-radius parameter 0.4
      // muons and neutrinos are excluded from the clustering

      // FinalState of prompt photons and bare muons and electrons in the event
      PromptFinalState photons(Cuts::abspid == PID::PHOTON);
      PromptFinalState bare_leps(Cuts::abspid == PID::MUON || Cuts::abspid == PID::ELECTRON);

      // Dress the prompt bare leptons with prompt photons within dR < 0.1,
      // and apply some fiducial cuts on the dressed leptons
      Cut lepton_cuts = Cuts::abseta < 2.5 && Cuts::pT > 20*GeV;
      DressedLeptons dressed_leps(photons, bare_leps, 0.1, lepton_cuts);
      declare(dressed_leps, "leptons");
    }


    /// Perform the per-event analysis
    void analyze(const Event& event)
    {

      // Retrieve dressed leptons, sorted by pT
      vector<DressedLepton> leptons = apply<DressedLeptons>(event, "leptons").dressedLeptons();

      // get event weight
      double weight = 1.2345;
      wtot_ += weight;
      // Compute Q2, W, theta, phi
      // Fill some control plot histos
      // find the bin
      double Q2 = 0.311;
      double W  = 1.2345;
      double Th = 156.37;
      double Ph = 271.99;

      int i = findBin(arrQ2, Q2,  5);
      int j = findBin(arrW,  W,  26);
      int k = findBin(arrTh, Th, 13);
      int l = findBin(arrPh, Ph, 13);

      if (i<0 or j<0 or k<0 or l<0) vetoEvent;
      printf("%d %d %d %d\n",i,j,k,l);

      // Fiducial cuts

      DATA_[i][j][k][l] += weight;
    }


    /// Normalise histograms etc., after the run
    void finalize()
    {
      // XS we can either read from HepMC or apply in post
      //
      std::ofstream myfile;
      // Could do output file name via std getenv
      myfile.open ("output.txt"); // Maybe write out dimensions as well
      myfile << "#4 25 12 12\n";
      

      // Divide all bins/cells by the total sum of weights
      for (int i=0;i<4;i++)
      for (int j=0;j<25;j++)
      for (int k=0;k<12;k++)
      for (int l=0;l<12;l++) 
      {
        DATA_[i][j][k][l] /= wtot_;
        // write to file 
        myfile << std::setprecision(8) <<  DATA_[i][j][k][l] << "\n";
      }
      myfile.close();
      
      //printf("%f %f\n", DATA_[0][6][10][9], wtot_);

      //normalize(_h["XXXX"]); // normalize to unity
      //normalize(_h["YYYY"], crossSection()/picobarn); // normalize to generated cross-section in fb (no cuts)
      //scale(_h["ZZZZ"], crossSection()/picobarn/sumW()); // norm to generated cross-section in pb (after cuts)

    }

    ///@}


    /// @name Histograms
    ///@{
    map<string, Histo1DPtr> _h;
    map<string, Profile1DPtr> _p;
    map<string, CounterPtr> _c;
    ///@}
    double DATA_[4][25][12][12];
    double wtot_;


  };


  RIVET_DECLARE_PLUGIN(CLAS_2006_I707883);

}
