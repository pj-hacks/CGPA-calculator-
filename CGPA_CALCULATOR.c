/*
author:JOSEPH PRINCE ANIEKEME 
CREATED ON : 3 / JULY/2024
FINISHED ON: 4/JULY/2024
DESCRIPTION: FOR THE CALCULATION OF CGPA 
*/

#include <stdio.h>

#define MAX_SUBJECT_LENGTH 50

int main()
{
	printf("THIS CALCULATOR WILL HELP YOU CALCULATE YOU CGPA: \n"
	       "THANKS YOU FOR WORKING WITH US.");

	int initial;

	printf("\nhow many courses:");
	if (scanf("%d", &initial) != 1){
		printf("invalid integer enter from 1 to infinity\n");
        
        return 0;
        }
    const int max_courses = 100;// it help prevent buffer overflow making sure that the user can not enter more than 99 characters.
	int mark[max_courses];//your score gotten from each subject
	char subject[max_courses][MAX_SUBJECT_LENGTH];//take and store the name of the subject which you want to use to calculate your cgpa
	int credit_load[max_courses];//take and store the credit load of the course provided by the user
    int total_credit_unit = 0;//the sum of all the credit point gotten from the scores
    int grand_point[max_courses];//the grade you get according to your score
    int quality_point[max_courses];//credit load * grand point
    int total_quality_point;//sum of all the quality piont from 1 to initial
    float CGPA;//to calculate cgpa
    
    

	for (int i = 0; i < initial; i++)
	{
		printf("\nwhats the %d subjects:",i+1);
		scanf("%s", subject[i]);

		printf("what was you score in %s:", subject[i]);
		scanf("%d", &mark[i]);

		printf("whats is the credit load of %s:", subject[i]);
		scanf("%d", &credit_load[i]);
        
       // total_course_credit += credit_load[i];
	
    //printf("\nThe course have a total of %d credit load",total_course_cgpa );
    
    
     if (mark[i] >= 70) {
        grand_point[i] = 5;
     } // grade A
     
     else if (mark[i] >= 60) {
        grand_point[i] = 4;
     }//grade B
     
      else if (mark[i] <= 59 || mark[i] >= 50) {
        grand_point[i] = 3;
     }//grade C
     
       else if (mark[i] <= 49 || mark[i] >= 45){
       grand_point[i] = 2;
     }//grade D
     
       else if (mark[i] <= 43 || mark[i] >= 40){
       grand_point[i] = 1;
     }//grade E
     
     else {
     grand_point[i] = 0;
     }//grade F

     total_credit_unit += grand_point[i];
     
     quality_point[i] = credit_load[i]  *  grand_point[i];
     
     total_quality_point += quality_point[i];
     
      CGPA = (float)total_quality_point / total_credit_unit;
}

printf("Therfore your CGPA = %.2f \n", CGPA);

	return 0;
}
