#ifndef SWITCHES_H
#define SWITCHES_H

// All switches are on port E
#define SW_UP_POS (21)
#define SW_DN_POS (29)
#define SW_LT_POS (30)
#define SW_RT_POS (23)
#define SW_CR_POS (22)

// Macro to read switches returns state switches, active low
#define READ_SWITCHES  (PTE->PDIR)  

// Function prototypes
extern void Init_5way_Switch(void);

#endif
// ********************************************************************   
