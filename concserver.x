struct Dianismata {
    int X<>;         /* διάνυσμα X */
    int Y<>;         /* διάνυσμα Y */
};

struct MesesTimes {
    double Ex;
    double Ey;
};

struct GinomenoEisodos {
    double r;
    int X<>;
};

struct GinomenoEpistrofi {
    double result<>;
};

program CONCSERVER_PROG {
    version CONCSERVER_VERS {
        /* 1. Εσωτερικό γινόμενο δύο διανυσμάτων */
        int GINOMENO_DIANISMATON(Dianismata) = 1;

        /* 2. Υπολογισμός μέσης τιμής διανυσμάτων */
        MesesTimes MESITIMI(Dianismata) = 2;

        /* 3. Γινόμενο r*X */
        GinomenoEpistrofi GINOMENO(GinomenoEisodos) = 3;
    } = 1;
} = 0x23451111;
