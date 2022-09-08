
    TYPE tagShot
        Ph AS LONG
        FileNumber AS LONG
        ZP AS LONG
        HP AS LONG
        BeginTrace AS LONG
        EndTrace AS LONG
        BeginGapTrace AS LONG
        EndGapTrace AS LONG
        XZB AS DOUBLE
        YZB AS DOUBLE
    END TYPE
    DIM Shot AS tagShot
    Shot.BeginTrace = 0
    Shot.EndTrace = 0
    Shot.BeginGapTrace = 0
    Shot.EndGapTrace = 0
    Shot.XZB = 0
    Shot.YZB = 0
  
    OPEN "swath000.spp" FOR OUTPUT AS #1


    FOR I = 1 TO 10
        FOR J = 1 TO 6
            n = (I - 1) * 6 + J
            Ph = (500 + I) * 1000 + J
            FileNumber = n
            PRINT #1, Ph; FileNumber; " 0 0 0 0 0 0 0.0 0.0"
            
        NEXT J
    NEXT I
    CLOSE
    END




