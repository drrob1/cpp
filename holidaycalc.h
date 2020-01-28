struct mdtype IS
         int m,d;
END;

struct HolType IS
         mdtype MLK,Pres,Easter,Mother,Memorial,Father,Labor,Columbus,Election,Thanksgiving;
         int year;
         bool valid;
  	   END;

PROCEDURE GetHolidays(HolType REF Holidays);
