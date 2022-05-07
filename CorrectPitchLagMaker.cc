#include <iostream.h>
#include <fstream.h>
#include <cmath>

#define defNumOfSamples         200000 /* VUV.txt $B$rFI$_9~$_$KI,MW$J%a%b%j$r3NJ](B */
#define defFrameShift           16
#define defUpperLimitOfPitchLag 400

//using namespace std;

class range{
  int Cnt;
public:
  void init();
  void increment();
  int GetCnt();
};

void range::init(){
  Cnt = 0;
}

void range::increment(){
  Cnt++;
}

int range::GetCnt(){
  return Cnt;
}

int main(int argc, char **argv){
  bool BoolFlgVoiced[defNumOfSamples], flgBreak = 0;
  int i = 0, j, Head, Tail, CorrectPitchLag, NumOfSamples, FrameShift = defFrameShift, NumOfFrames, \
    UpperLimitOfPitchLag = defUpperLimitOfPitchLag, IntFlgPeak[defNumOfSamples], CntOfInterval = 0;
  double CorrectPitchHz, MaxPitch = -1, MinPitch = 100000;
  range under75, over75under150, over150under300, over300;

  
  ifstream fpInPeakFile("PeakFile.txt");
  if(!fpInPeakFile){
    cerr << "Cannot open PeakFile.txt.\n";
    return 1;
  }

  ifstream fpInVuvFile("VuvFile.txt");
  if(!fpInVuvFile){
    cerr << "Cannot open VuvFile.txt.\n";
    return 1;
  }

  i = 0;
  while(!fpInPeakFile.eof()){
    /* Int$B7?$GJ];}(B */
    fpInPeakFile >> IntFlgPeak[i];
    /* Bool$B7?$GJ];}(B */
    fpInVuvFile >> BoolFlgVoiced[i];
    //    cout << IntFlgPeak[i] << endl;
    i++;
  }
  fpInPeakFile.close();
  fpInVuvFile.close();
  NumOfSamples = i; /* $BA4$F$N%5%s%W%k$N?t(B + 1 */

  /* $B%G!<%?%A%'%C%/(B */
  for(i = 0; i < NumOfSamples; i++){
    if(0 < IntFlgPeak[i]){
      if(CntOfInterval < 40){
	cerr << " Interval Erorr : Line " << i << "   Interval : " << CntOfInterval << endl;
      }
      CntOfInterval = 0;
    }
    CntOfInterval++;
  }
  /***/

  /* $B=i4|2=(B */
  under75.init();
  over75under150.init();
  over150under300.init();
  over300.init();


  /* j $B$O(BIntFlgPeak[] == 0 $B$G$"$k8D?t$KAjEv$9$k(B */
  for(i = 0; i <= (double)NumOfSamples/FrameShift; i++){
    flgBreak = 0;
    /* $BL$MhJ}8~$K8~$+$C$F$9$9$`(B */
    j = 0;
    while(IntFlgPeak[i*FrameShift+j] <= 0){
      /* $BL5@<6h4V$K$G$/$o$7$?$H$-(B              $B%T!<%/%U%i%0$,Ii$N$H$-(B            $B%U%!%$%k$N:G8e$^$G$$$C$?$H$-(B */
      if(BoolFlgVoiced[i*FrameShift+j] == 0 || IntFlgPeak[i*FrameShift+j] < 0 || i*FrameShift+j == NumOfSamples - 1){
	flgBreak = 1;
	break;
      }
      j++;
    }
    if(!flgBreak){
      /* Head = j $B$G4V0c$$$J$$!%(B*/
      Head = j;
      j = 0;
      while(IntFlgPeak[i*FrameShift-1-j] <= 0){
	/* $BL5@<6h4V$K$G$/$o$7$?$H$-(B              $B%T!<%/%U%i%0$,Ii$N$H$-(B            $B%U%!%$%k$N:G=i$^$G$$$C$?$H$-(B */
	if(BoolFlgVoiced[i*FrameShift-1-j] == 0 || IntFlgPeak[i*FrameShift-1-j] < 0 || i*FrameShift-1-j == 0){
	  flgBreak = 1;
	  break;
	}
	j++;
      }
    }
    /* Break $B$7$?%U%l!<%`$N%T%C%A%i%0$O(B 0 $B$K$9$k(B */
    if(!flgBreak){
      /* Tail = j $B$G4V0c$$$J$$(B */
      Tail = j;
      /* Head Tail $B$O$=$l$>$lA0$H8e$m$N(B 0 $B$N8D?t$@$+$i(B $B$3$l$K(B 1 $B$r2C$($F%T%C%A%i%0$H$9$k(B */
      CorrectPitchLag = Head+Tail+1;
    }
    else{
      CorrectPitchLag = 0;
    }

    /* $B%T%C%A%i%0$,Bg$-$9$.$k$H$-$O(B 0 $B$K$9$k(B */
    if(UpperLimitOfPitchLag < CorrectPitchLag){
      CorrectPitchLag = 0;
    }

    /* $BC10L$r(B Hz $B$K$7$F(B CorrectPitch $B$KBeF~(B */
    if(0 != CorrectPitchLag) CorrectPitchHz = 16000.0/CorrectPitchLag;
    else CorrectPitchHz = 0;

    
    if(0 != CorrectPitchLag){
      if(CorrectPitchHz < MinPitch) MinPitch = CorrectPitchHz;
      else if(CorrectPitchHz > MaxPitch) MaxPitch = CorrectPitchHz;
    }
      
    if(75 > CorrectPitchHz && 0 != CorrectPitchHz){
      under75.increment();
    }
    else if(150 > CorrectPitchHz){
      over75under150.increment();
    }
    else if(300 > CorrectPitchHz){
      over150under300.increment();
    }
    else{
      over300.increment();
    }
    
    cout << CorrectPitchHz << endl;

  } /* for(i = 0; i <= (double)NumOfSamples/FrameShift; i++){ */
  
  NumOfFrames = under75.GetCnt()+over75under150.GetCnt()+over150under300.GetCnt()+over300.GetCnt();
  cerr << under75.GetCnt() << " " << over75under150.GetCnt() << " " << over150under300.GetCnt() << " " << over300.GetCnt() << endl;
  cerr << 100.*under75.GetCnt()/NumOfFrames << " " << 100.*over75under150.GetCnt()/NumOfFrames << " " << 100.*over150under300.GetCnt()/NumOfFrames << " " << 100.*over300.GetCnt()/NumOfFrames << endl;
  cerr << "Min = " << MinPitch << "   Max = " << MaxPitch << endl;
  return 0;

}
