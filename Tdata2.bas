
    TYPE tagFstBrk
        FileNumber AS INTEGER
        FirstBreak AS INTEGER
    END TYPE
    DIM FstBrk AS tagFstBrk
  
    OPEN "swath000.FBD" FOR BINARY AS #1
    FOR I = 1 TO 10
        FOR J = 1 TO 6
            n = (I - 1) * 6 + J
            FOR K = 1 TO 124
                FstBrk.FileNumber = n
                FstBrk.FirstBreak = K * 4
                PUT #1, , FstBrk
            NEXT K
        NEXT J
    NEXT I
    CLOSE
    END




