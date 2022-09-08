
    DIM Ph(692), V(692)
    OPEN "hyr.txt" FOR INPUT AS #1
    FOR I = 1 TO 5
        OPEN "R" + LTRIM$(RTRIM$(STR$(I))) + ".txt" FOR OUTPUT AS #2
        FOR J = 1 TO 692
            INPUT #1, Ph(J), V(J)
        NEXT J
        FOR J = 692 TO 1 STEP -1
            PRINT #2, Ph(J), -CINT(V(J))
        NEXT J
        CLOSE #2
        PRINT I
    NEXT I
    CLOSE
    END




