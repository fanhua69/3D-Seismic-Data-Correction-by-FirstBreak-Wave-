
    #include "stdio.h"
    main()
    {
        char JBFile[256];
        char OutFile[256];
        int i,n,JBShot;
        char group[2512];
        long FileNumber,ByteShot;
        FILE *fpJB,*fpShotLine,*fpFileName;

        // Give file name here:
        JBFile=" ";
        FileNumberFile=" "
        OutFile=" ";

        // Get the shot point number here:
        ByteShot=2512*410;
        fpJB=fopen(JBFile,"r");
        fseek(fpJB,0,2);
        i=ftell(fpJB);
        JBShot=i/(2512*410);

        fpShotLine=fopen(OutFile,"w");
        fpFileName=fopen(FileNumberFile,"r");

        for(shot=0;shot<20;shot++){
            fscanf(fpFileName,"%ld\n",&FileNumber);

            for(i=0;i<JBShot;i++){
                pos=i*ByteShot+4;
                fseek(fpJB,pos,0)
                fread(&n,sizeof(long),1,fpJB);
                if(n==FileNumber){
                    pos=i*ByteShot;
                    fseek(fpJB,pos,0);
                    fread(group,sizeof(char),2512,fpJB);
                    fwrite(group,sizeof(char),2512,fpShotLine);
                }
            }
        }

        fclose(fpShotLine);
        fclose(fpJB);
        return;
   }



            
            



    JBFile
