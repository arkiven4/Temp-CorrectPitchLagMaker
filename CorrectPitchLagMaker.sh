#! /bin/tcsh -f

set DirOfInputFile = ~/project/research/pitch/assignment/NAIST-CREST/PeakandVuv
set DirOfOutputFile = ~/project/research/pitch/assignment/reference/pitch/CorrectPitch-Hz
 
#cp /home/oak/tanaka/RightPitch/m/m01/m01-4-pit.txt ./INPUT.txt
#cp /home/oak/tanaka/CorrectPitchLag/m01/m01-1-pit.txt ./INPUT.txt

foreach Sex ( m f )
  @ NameCode = 1
  while( $NameCode <= 14 )
    if( $NameCode < 10 ) then
      set NameCode = 0$NameCode
    endif
    set Name = ${Sex}${NameCode}
    
    if(! -d ${DirOfOutputFile}/${Name}) mkdir ${DirOfOutputFile}/${Name} # ディレクトリ作成

    foreach Num ( 1 2 3 )
      echo "${Name}-${Num}-pit.txt"
      cp ${DirOfInputFile}/${Sex}/${Name}/${Name}-${Num}-pit.txt ./PeakFile.txt
      cp ${DirOfInputFile}/${Sex}/${Name}/${Name}-${Num}-vuv.txt ./VuvFile.txt
      ./CorrectPitchLagMaker \
      > ! ${DirOfOutputFile}/${Name}/${Name}-${Num}-pit.txt
    
      rm PeakFile.txt VuvFile.txt

    end
    @ NameCode ++
  end
end


