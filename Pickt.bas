
    DIM Ph(7000), V(7000)

    OPEN "rmid002.dat" FOR INPUT AS #1
    n = 0
    DO WHILE NOT EOF(1)
        n = n + 1
        INPUT #1, Ph(n), V(n)
    LOOP
    Number = n
    CLOSE

    OPEN "r630.fbk" FOR OUTPUT AS #1
    FOR I = 106 TO 115
        IF I = 115 THEN STOP
        FOR J = 617 + 9 TO 617 + 49
            Ph = I * 1000 + J
            FOR K = 1 TO Number
                IF Ph(K) = Ph THEN
                    PRINT #1, INT(V(K))
                    PRINT Ph, V(K)
                    EXIT FOR
                END IF
            NEXT K
        NEXT J
    NEXT I
    CLOSE
    END

