#include <iostream.h>
#include <fstream.h>
#include <cmath>

#define defNumOfSamples         200000 /* VUV.txt を読み込みに必要なメモリを確保 */
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
    /* Int型で保持 */
    fpInPeakFile >> IntFlgPeak[i];
    /* Bool型で保持 */
    fpInVuvFile >> BoolFlgVoiced[i];
    //    cout << IntFlgPeak[i] << endl;
    i++;
  }
  fpInPeakFile.close();
  fpInVuvFile.close();
  NumOfSamples = i; /* 全てのサンプルの数 + 1 */

  /* データチェック */
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

  /* 初期化 */
  under75.init();
  over75under150.init();
  over150under300.init();
  over300.init();


  /* j はIntFlgPeak[] == 0 である個数に相当する */
  for(i = 0; i <= (double)NumOfSamples/FrameShift; i++){
    flgBreak = 0;
    /* 未来方向に向かってすすむ */
    j = 0;
    while(IntFlgPeak[i*FrameShift+j] <= 0){
      /* 無声区間にでくわしたとき              ピークフラグが負のとき            ファイルの最後までいったとき */
      if(BoolFlgVoiced[i*FrameShift+j] == 0 || IntFlgPeak[i*FrameShift+j] < 0 || i*FrameShift+j == NumOfSamples - 1){
	flgBreak = 1;
	break;
      }
      j++;
    }
    if(!flgBreak){
      /* Head = j で間違いない．*/
      Head = j;
      j = 0;
      while(IntFlgPeak[i*FrameShift-1-j] <= 0){
	/* 無声区間にでくわしたとき              ピークフラグが負のとき            ファイルの最初までいったとき */
	if(BoolFlgVoiced[i*FrameShift-1-j] == 0 || IntFlgPeak[i*FrameShift-1-j] < 0 || i*FrameShift-1-j == 0){
	  flgBreak = 1;
	  break;
	}
	j++;
      }
    }
    /* Break したフレームのピッチラグは 0 にする */
    if(!flgBreak){
      /* Tail = j で間違いない */
      Tail = j;
      /* Head Tail はそれぞれ前と後ろの 0 の個数だから これに 1 を加えてピッチラグとする */
      CorrectPitchLag = Head+Tail+1;
    }
    else{
      CorrectPitchLag = 0;
    }

    /* ピッチラグが大きすぎるときは 0 にする */
    if(UpperLimitOfPitchLag < CorrectPitchLag){
      CorrectPitchLag = 0;
    }

    /* 単位を Hz にして CorrectPitch に代入 */
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
