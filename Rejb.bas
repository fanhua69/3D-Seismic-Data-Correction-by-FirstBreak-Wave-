
    SCREEN 12
    VIEW (0, 0)-(639, 479)
    WINDOW SCREEN (0, 0)-(12 * 41, 500)
    OPEN "jb01" FOR RANDOM AS #1 LEN = 628 * 4
    FIELD #1, 628 * 4 AS A$
    OPEN "j01" FOR RANDOM AS #2 LEN = 628 * 4
    FIELD #2, 628 * 4 AS B$
   
    FOR I = 102 TO 102
        P1 = (I - 1) * 410
        P2 = (I - 1) * 310
                       
        FOR Group = 8 TO 10
            PP1 = P1 + (Group - 1) * 41
            PP2 = P2 + (Group - 1) * 31
            FOR J = 1 TO 31
                GET #1, PP1 + J + 1
                LSET B$ = A$
                PUT #2, PP2 + J
            NEXT J
        NEXT Group
        PRINT I
    NEXT I
    
    A$ = INPUT$(1)
    END





