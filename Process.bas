
    DIM Ph(3500), Value(3500)
    OPEN "s.dat" FOR INPUT AS #1
    n = 0
    DO WHILE NOT EOF(1)
        n = n + 1
        INPUT #1, Ph(n), X, Y, Value(n)
    LOOP
    SNumber = n
    CLOSE #1

    DIM FileNoGroup(3500)
    OPEN "swath2.fn" FOR INPUT AS #1
    n = 0
    DO WHILE NOT EOF(1)
        n = n + 1
        INPUT #1, FileNumber
        ShotLine = (n - 1) \ 20
        ShotPoint = (n - 1) MOD 20 + 121
        Phthis = (650 - ShotLine) * 1000 + ShotPoint
        FOR I = 1 TO SNumber
            IF Ph(I) = Phthis THEN
                FileNoGroup(FileNumber) = Value(I)
                EXIT FOR
            END IF
        NEXT I
        IF n MOD 100 = 0 THEN PRINT n
    LOOP
    FFN = n
    CLOSE #1

    OPEN "hy.txt" FOR OUTPUT AS #1
    FOR I = 1 TO FFN
        PRINT #1, I; -CINT(FileNoGroup(I))
        'IF I MOD 10 = 0 THEN PRINT #1,
    NEXT I
    CLOSE #1

    END












