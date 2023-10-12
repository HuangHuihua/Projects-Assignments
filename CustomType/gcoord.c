/*
 * src/tutorial/gcoord.c
 *
 ******************************************************************************
  This file contains routines that can be bound to a Postgres backend and
  called by the backend in the process of processing queries.  The calling
  format for these routines is dictated by Postgres architecture.
******************************************************************************/

#include "postgres.h"

#include "fmgr.h"
#include "libpq/pqformat.h"		/* needed for send/recv functions */

#include <ctype.h>

#include <math.h>

#include <stdlib.h>
#include<stdio.h>

#define LEN 256

PG_MODULE_MAGIC;

	typedef struct Latitude{
		int coordinatevaluei;
		int coordinatevaluef;
		char direction;
	} Latitude;

	typedef struct Longitude{
		int coordinatevaluei;
		int coordinatevaluef;
		char direction;
	} Longitude;


typedef struct GeoCoord
{
	char LocationName[LEN];
	Latitude lat;
	Longitude lon;

}			GeoCoord;




/*****************************************************************************
 * Input/Output functions
 *****************************************************************************/

PG_FUNCTION_INFO_V1(geocoord_in);

Datum
geocoord_in(PG_FUNCTION_ARGS)
{
	// int 		id = PG_GETARG_INT(0);
	char	   *str = PG_GETARG_CSTRING(0);
	// int 		index;//index
	char 		name[LEN];	
	char 		NS,
				WE;
	char		xfi[5],
				yfi[5];
	// char		unit;
	int			x = 0,
				xf =0,
				yf = 0,
				y = 0;
	// double      test1,
	// 			test2;

	GeoCoord    *result;

// if(sscanf(str, "%[^,],%lf°%c,%lf°%c", name, &test1, &NS, &test2, &WE) != 5){
// 				ereport(ERROR,
// 						(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
// 						errmsg("invalid input syntax for type1 %s: \"%s\"",
// 								"GeoCoord", str)));
// }
	if ((sscanf(str, "%[^,],%d.%[^°]°%c,%d.%[^°]°%c", name, &x,xfi, &NS, &y,yfi, &WE) != 7  && sscanf(str, "%[^,],%d.%[^°]°%c %d.%[^°]°%c",  name, &x,xfi, &NS, &y,yfi, &WE) != 7) ){
		if((sscanf(str, "%[^,],%d°%[^ ] %d.%[^°]°%c",  name, &x, &NS, &y,yfi, &WE) != 6 && sscanf(str, "%[^,],%d.%[^°]°%[^ ] %d°%c",  name, &x,xfi, &NS, &y, &WE) != 6 ) && (sscanf(str, "%[^,],%d°%c,%d.%[^°]°%c", name, &x, &NS, &y,yfi, &WE) != 6&&sscanf(str, "%[^,],%d.%[^°]°%c,%d°%c", name, &x,xfi, &NS, &y, &WE) != 6) ){
			if(sscanf(str, "%[^,],%d°%c,%d°%c", name, &x, &NS, &y, &WE) != 5  && sscanf(str, "%[^,],%d°%[^ ] %d°%c",  name, &x, &NS, &y, &WE) != 5){
				ereport(ERROR,
						(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
						errmsg("invalid input syntax for type1 %s: \"%s\"",
								"GeoCoord", str)));
}}}
		// if(xf<10 && xf >0) xf = xf*1000;
		// else if(xf>=10 && xf <100) xf = xf*100;
		// else if(xf>=100 && xf <1000) xf = xf*10;
		// else  xf = xf;

		// if(yf<10 && yf >0) yf = yf*1000;
		// else if(yf>=10 && yf <100) yf = yf*100;
		// else if(yf>=100 && yf <1000) yf = yf*10;
		// else  yf = yf;

		for(int k = 0; k<5 ;k++){
			// if(xfi[k] != '0' && xfi[k] != '1' && xfi[k] != '2' && xfi[k] != '3' && xfi[k] != '4' && xfi[k] != '5' && xfi[k] != '6' && xfi[k] != '7' && xfi[k] != '8' &&xfi[k] != '9' && xfi[k] != '\0') 
			// 		ereport(ERROR,
			// 			(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
			// 			errmsg("invalid input syntax for float %s: \"%s\"",
			// 					"GeoCoord", str)));

			// if(yfi[k] != '0' && yfi[k] != '1' && yfi[k] != '2' && yfi[k] != '3' && yfi[k] != '4' && yfi[k] != '5' && yfi[k] != '6' && yfi[k] != '7' && yfi[k] != '8' &&yfi[k] != '9' && yfi[k] != '\0') 
			// 		ereport(ERROR,
			// 			(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
			// 			errmsg("invalid input syntax for float %s: \"%s\"",
			// 					"GeoCoord", str)));			
			if(xfi[k] == '.' || yfi[k] == '.')
					ereport(ERROR,
						(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
						errmsg("invalid input syntax for float %s: \"%s\"",
								"GeoCoord", str)));		
		}


		if(xfi[0] == '0'){
			if(xfi[1] == '0'){
				if(xfi[2] == '0'){
					if(xfi[3] == '0'){
						xf = 0;
					}
					else if(xfi[3] == '\0'){
						xf = 0;
					}
					else// 3 not 0
					{
						xf = atoi(xfi);
					}
				}
				else if(xfi[2] == '\0'){
					//this case no next digit
					xf = 0;
				}
				else //2 not 0
				{
					//xf = atoi(xfi)*10;
					if(xfi[3] == '0'){
						xf = atoi(xfi);
					}
					else if(xfi[3] == '\0'){
						xf = atoi(xfi)*10;
					}
					else// 3 not 0
					{
						xf = atoi(xfi);
					}
				}
			}
			else if(xfi[1] == '\0'){
				xf = 0;
			}
			else
			{// 1 not 0 
				if(xfi[2] == '0'){
					if(xfi[3] == '0'){
						xf = atoi(xfi);
					}
					else if(xfi[3] == '\0'){
						xf = atoi(xfi)*10;
					}
					else// 3 not 0
					{
						xf = atoi(xfi);
					}
				}
				else if(xfi[2] == '\0'){
					//this case no next digit
					xf = atoi(xfi)*100;
				}
				else //2 not 0
				{
					//xf = atoi(xfi)*10;
					if(xfi[3] == '0'){
						xf = atoi(xfi);
					}
					else if(xfi[3] == '\0'){
						xf = atoi(xfi)*10;
					}
					else// 3 not 0
					{
						xf = atoi(xfi);
					}
				}
			}
		}else if(xfi[0] == '\0'){

			xf = 0;

		}else{ // 0 not 0
			//xf = atoi(xfi)*1000;
			if(xfi[1] == '0'){
				if(xfi[2] == '0'){
					if(xfi[3] == '0'){
						xf = atoi(xfi);
					}
					else if(xfi[3] == '\0'){
						xf = atoi(xfi)*10;
					}
					else// 3 not 0
					{
						xf = atoi(xfi);
					}
				}
				else if(xfi[2] == '\0'){
					//this case no next digit
					xf = atoi(xfi)*100;
				}
				else //2 not 0
				{
					//xf = atoi(xfi)*10;
					if(xfi[3] == '0'){
						xf = atoi(xfi);
					}
					else if(xfi[3] == '\0'){
						xf = atoi(xfi)*10;
					}
					else// 3 not 0
					{
						xf = atoi(xfi)*100;
					}
				}
			}
			else if(xfi[1] == '\0'){
				xf = atoi(xfi)*1000;
			}
			else
			{// 1 not 0 
				if(xfi[2] == '0'){
					if(xfi[3] == '0'){
						xf = atoi(xfi);
					}
					else if(xfi[3] == '\0'){
						xf = atoi(xfi)*10;
					}
					else// 3 not 0
					{
						xf = atoi(xfi);
					}
				}
				else if(xfi[2] == '\0'){
					//this case no next digit
					xf = atoi(xfi)*100;
				}
				else //2 not 0
				{
					//xf = atoi(xfi)*10;
					if(xfi[3] == '0'){
						xf = atoi(xfi);
					}
					else if(xfi[3] == '\0'){
						xf = atoi(xfi)*10;
					}
					else// 3 not 0
					{
						xf = atoi(xfi);
					}
				}
			}
		}


		if(yfi[0] == '0'){
			if(yfi[1] == '0'){
				if(yfi[2] == '0'){
					if(yfi[3] == '0'){
						yf = 0;
					}
					else if(yfi[3] == '\0'){
						yf = 0;
					}
					else// 3 not 0
					{
						yf =atoi(yfi);
					}
				}
				else if(yfi[2] == '\0'){
					//this case no next digit
					yf = 0;
				}
				else //2 not 0
				{
					//yf = atoi(yfi)*10;
					if(yfi[3] == '0'){
						yf = atoi(yfi);
					}
					else if(yfi[3] == '\0'){
						yf = atoi(yfi)*10;
					}
					else// 3 not 0
					{
						yf = atoi(yfi);
					}
				}
			}
			else if(yfi[1] == '\0'){
				yf = 0;
			}
			else
			{// 1 not 0 
				if(yfi[2] == '0'){
					if(yfi[3] == '0'){
						yf = atoi(yfi);
					}
					else if(yfi[3] == '\0'){
						yf = atoi(yfi)*10;
					}
					else// 3 not 0
					{
						yf = atoi(yfi);
					}
				}
				else if(yfi[2] == '\0'){
					//this case no next digit
					yf = atoi(yfi)*100;
				}
				else //2 not 0
				{
					//yf = atoi(yfi)*10;
					if(yfi[3] == '0'){
						yf = atoi(yfi);
					}
					else if(yfi[3] == '\0'){
						yf = atoi(yfi)*10;
					}
					else// 3 not 0
					{
						yf = atoi(yfi);
					}
				}
			}
		}else if(yfi[0] == '\0'){

			yf = 0;

		}else{ // 0 not 0
			//yf = atoi(yfi)*1000;
			if(yfi[1] == '0'){
				if(yfi[2] == '0'){
					if(yfi[3] == '0'){
						yf = atoi(yfi);
					}
					else if(yfi[3] == '\0'){
						yf = atoi(yfi)*10;
					}
					else// 3 not 0
					{
						yf = atoi(yfi);
					}
				}
				else if(yfi[2] == '\0'){
					//this case no next digit
					yf = atoi(yfi)*100;
				}
				else //2 not 0
				{
					//yf = atoi(yfi)*10;
					if(yfi[3] == '0'){
						yf = atoi(yfi);
					}
					else if(yfi[3] == '\0'){
						yf = atoi(yfi)*10;
					}
					else// 3 not 0
					{
						yf = atoi(yfi)*100;
					}
				}
			}
			else if(yfi[1] == '\0'){
				yf = atoi(yfi)*1000;
			}
			else
			{// 1 not 0 
				if(yfi[2] == '0'){
					if(yfi[3] == '0'){
						yf = atoi(yfi);
					}
					else if(yfi[3] == '\0'){
						yf = atoi(yfi)*10;
					}
					else// 3 not 0
					{
						yf = atoi(yfi);
					}
				}
				else if(yfi[2] == '\0'){
					//this case no next digit
					yf = atoi(yfi)*100;
				}
				else //2 not 0
				{
					//yf = atoi(yfi)*10;
					if(yfi[3] == '0'){
						yf = atoi(yfi);
					}
					else if(yfi[3] == '\0'){
						yf = atoi(yfi)*10;
					}
					else// 3 not 0
					{
						yf = atoi(yfi);
					}
				}
			}
		}



	if(NS != 'N' && NS != 'S' && NS != 'W' && NS != 'E')
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type2 %s: \"%s\"",
						"GeoCoord", str)));
						//  psprintf("The latitude is not in right format");
	if(WE != 'N' && WE != 'S' && WE != 'W' && WE != 'E')
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type3 %s: \"%s\"",
						"GeoCoord", str))); 
						// psprintf("The longtitude is not in right format");

	if(NS == 'N' || NS == 'S'){
		if(WE == 'N' || WE == 'S')
			ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type4 %s: \"%s\"",
						"GeoCoord", str))); 
	}

	if(NS == 'W' || NS == 'E'){
		if(WE == 'W' || WE == 'E')
			ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type5 %s: \"%s\"",
						"GeoCoord", str))); 
	}
		
if(NS == 'N' || NS == 'S'){
	if(x>90 || x <0) 
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type6 %s: \"%s\"",
						"GeoCoord", str))); 
	if(y>180 || y <0) 
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type7 %s: \"%s\"",
						"GeoCoord", str))); 	
}

if(NS == 'W' || NS == 'E'){
	if(x>180  || x <0) 
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type8 %s: \"%s\"",
						"GeoCoord", str))); 	
	if(y>90 || y <0) 
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type9 %s: \"%s\"",
						"GeoCoord", str))); 
}

	// if(y>180 || y <0) 
	// 	ereport(ERROR,
	// 			(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
	// 			 errmsg("invalid input syntax for type %s: \"%s\"",
	// 					"GeoCoord", str))); 


	for(int i = 0; i < LEN; ++i){
		// if(name[i]>255 || name[i]<0)
		// 	ereport(ERROR,
		// 			(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
		// 			errmsg("invalid input syntax for type %s: \"%s\"",
		// 					"GeoCoord", str)));

	if((name[i]=='-' && name[i+1]!='\0')&&name[i-1]!='\0'){
		if(((name[i+1]<=122 && name[i+1]>=97)||(name[i+1]<=90 && name[i+1]>=65))&&((name[i-1]<=122 && name[i-1]>=97)||(name[i-1]<=90 && name[i-1]>=65)))
					ereport(ERROR,
							(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
							errmsg("invalid input syntax for type22222 %s: \"%s\"",
									"GeoCoord", str)));


	}

	if(!((name[i]<=122 && name[i]>=97)||(name[i]<=90 && name[i]>=65))){
		// if(name[i]!=32){
		// 		if(name[i]=='-'&&name[i+1]!='\0')
		// 			ereport(ERROR,
		// 					(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
		// 					errmsg("invalid input syntax for type22222 %s: \"%s\"",
		// 							"GeoCoord", str)));
		// }
		// else 
		
		if(name[i]==32) {
			if(name[i+1] >122)
				ereport(ERROR,
					(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
					errmsg("There is space in the end1 %s: \"%s\"",
							"GeoCoord", str)));			
			else if((name[i+1]<97 && name[i+1]>90) && (name[i+1]!='\0' ) )//&& name[i+2]!=0
				ereport(ERROR,
					(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
					errmsg("There is space in the end2 %s: \"%s\"",
							"GeoCoord", str)));		

			// if(name[i+1]<65 && ((name[i+1]!=48 && name[i+1]!=47)&&name[i+1]!=44))	
			// 	ereport(ERROR,
			// 		(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
			// 		errmsg("There is space in the end3 %s: \"%s\"",
			// 				"GeoCoord", str)));	
			if(name[i+1]=='\0')	
				ereport(ERROR,
					(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
					errmsg("There is space in the end3 %s: \"%s\"",
							"GeoCoord", str)));	
			
			if(name[i+1]==32)	
				ereport(ERROR,
					(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
					errmsg("There is space in the end4 %s: \"%s\"",
							"GeoCoord", str)));											

		}	
	}

		// if((name[i]>90 && name[i]< 97)||(name[i]<65 && name[i]!=32)||name[i]>122)
		// 	ereport(ERROR,
		// 			(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
		// 			errmsg("invalid input syntax for type %s: \"%s\"",
		// 					"GeoCoord", str)));
		
	}

	if(name[0]==' ')
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				errmsg("The beginning is space %s: \"%s\"",
						"GeoCoord", str))); 

		

	result = (GeoCoord *) palloc(sizeof(GeoCoord));
	strcpy(result->LocationName,name);
	result->lat.coordinatevaluei = x;
	result->lat.coordinatevaluef = xf;
	result->lon.coordinatevaluei = y;
	result->lon.coordinatevaluef = yf;	
	result->lat.direction = NS;
	result->lon.direction = WE;

	PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(geocoord_out);

Datum
geocoord_out(PG_FUNCTION_ARGS)
{
	GeoCoord    *geoCoord = (GeoCoord *) PG_GETARG_POINTER(0);
	// int			id = 1;
	char	   *result;
	int			i = 1;

	if(1){
		// geoCoord->LocationName[0] = toupper(geoCoord->LocationName[0]);
		
		while(i != LEN-1){
			geoCoord->LocationName[i]=tolower(geoCoord->LocationName[i]);
			// if(geoCoord->LocationName[i-1]==' '){
			// 	geoCoord->LocationName[i]= toupper(geoCoord->LocationName[i]);
			// }	
			++i;
		}
	}



	if(geoCoord->lat.direction == 'W' || geoCoord->lat.direction=='E'){


		result = psprintf("%s, %d.%04d°%c, %d.%04d°%c", geoCoord->LocationName, geoCoord->lat.coordinatevaluei,geoCoord->lat.coordinatevaluef, geoCoord->lat.direction, geoCoord->lon.coordinatevaluei,geoCoord->lon.coordinatevaluef, geoCoord->lon.direction);

	}else{
		result = psprintf("%s, %d.%04d°%c, %d.%04d°%c", geoCoord->LocationName, geoCoord->lon.coordinatevaluei,geoCoord->lon.coordinatevaluef, geoCoord->lon.direction, geoCoord->lat.coordinatevaluei,geoCoord->lat.coordinatevaluef, geoCoord->lat.direction);
	}
	//result = psprintf("%s, %g°%c, %g°%c", geoCoord->LocationName, geoCoord->lat.coordinatevalue, geoCoord->lat.direction, geoCoord->lon.coordinatevalue, geoCoord->lon.direction);
	PG_RETURN_CSTRING(result);
}



/*****************************************************************************
 * Operator class for defining B-tree index
 *
 * It's essential that the comparison operators and support function for a
 * B-tree index opclass always agree on the relative ordering of any two
 * data values.  Experience has shown that it's depressingly easy to write
 * unintentionally inconsistent functions.  One way to reduce the odds of
 * making a mistake is to make all the functions simple wrappers around
 * an internal three-way-comparison function, as we do here.
 *****************************************************************************/


static int
geocoord_abs_cmp_internal(GeoCoord * a, GeoCoord * b)
{
	// strcpy(result->LocationName,name);
	// result->lat.coordinatevalue = x;
	// result->lon.coordinatevalue = y;
	// result->lat.direction = NS;
	// result->lon.direction = WE;
	int		counta=0,
			countb=0;

	if(a->lat.direction == 'N' || a->lat.direction == 'S'){//a has left latitude, right longi

		if(b->lat.direction == 'N' || b-> lat.direction =='S'){//b also left lat, right longi
			if(a->lat.coordinatevaluei > b->lat.coordinatevaluei){
				return -1;
			}else if(a->lat.coordinatevaluei < b->lat.coordinatevaluei){
				return 1;
			}else if(a->lat.coordinatevaluei == b->lat.coordinatevaluei){//latitude equal, we check longitude
			if(a->lat.coordinatevaluef > b->lat.coordinatevaluef){
				return -1;
			}else if(a->lat.coordinatevaluef < b->lat.coordinatevaluef){
				return 1;
			}
			else if(a->lat.coordinatevaluef == b->lat.coordinatevaluef){
				if(a->lon.coordinatevaluei > b->lon.coordinatevaluei ){
					return -1;
				}else if(a->lon.coordinatevaluei < b->lon.coordinatevaluei){
					return 1;
				}else if(a->lon.coordinatevaluei == b->lon.coordinatevaluei){//if both lat and lon equal, we check name lenght
					if(a->lon.coordinatevaluef > b->lon.coordinatevaluef){
						return -1;
					}else if(a->lon.coordinatevaluef < b->lon.coordinatevaluef){
						return 1;
					}
					else if(a->lon.coordinatevaluef == b->lon.coordinatevaluef){
								
						counta=0,
						countb=0;
						for(int i = 0; i < LEN; ++i){
						if(a->LocationName[i]<255 && a->LocationName[i]>0){
							++counta;
						}
						if(b->LocationName[i]<255 && b->LocationName[i]>0){
							++countb;
						}
					}
					if(counta < countb) return -1;
					else if(counta > countb) return 1;
					else return 0;
				}
				}
			}
			}
		}else{//b has lef longi, right lat
			if(a->lat.coordinatevaluei > b->lon.coordinatevaluei){
				return -1;
			}else if(a->lat.coordinatevaluei < b->lon.coordinatevaluei){
				return 1;
			}else if(a->lat.coordinatevaluei == b->lon.coordinatevaluei){//latitude equal, we check longitude
				
				if(a->lat.coordinatevaluef > b->lon.coordinatevaluef){
					return -1;
				}else if(a->lat.coordinatevaluef < b->lon.coordinatevaluef){
					return 1;
				}
				else if(a->lat.coordinatevaluef == b->lon.coordinatevaluef){
				
				if(a->lon.coordinatevaluei > b->lat.coordinatevaluei ){
					return -1;
				}else if(a->lon.coordinatevaluei < b->lat.coordinatevaluei){
					return 1;
				}else if(a->lon.coordinatevaluei == b->lat.coordinatevaluei){//if both lat and lon equal, we check name lenght
						
					if(a->lon.coordinatevaluef > b->lat.coordinatevaluef){
						return -1;
					}else if(a->lon.coordinatevaluef < b->lat.coordinatevaluef){
						return 1;
					}
					else if(a->lon.coordinatevaluef == b->lat.coordinatevaluef){
					
						
						counta = 0;
						countb = 0;
						for(int i = 0; i < LEN; ++i){
						if(a->LocationName[i]<255 && a->LocationName[i]>0){
							++counta;
						}
						if(b->LocationName[i]<255 && b->LocationName[i]>0){
							++countb;
						}
					}
					if(counta < countb) return -1;
					else if(counta > countb) return 1;
					else return 0;
					
					}
				}
				}
			}
				
		}

	}
	else {//if a has left lon but right lat

		if(b->lat.direction == 'N' || b-> lat.direction == 'S'){// in this case, if b has left lat but right lon
			if(a->lon.coordinatevaluei > b->lat.coordinatevaluei){
				return -1;
			}else if(a->lon.coordinatevaluei < b->lat.coordinatevaluei){
				return 1;
			}else if(a->lon.coordinatevaluei == b->lat.coordinatevaluei){//latitude equal, we check longitude

				if(a->lon.coordinatevaluef > b->lat.coordinatevaluef){
					return -1;
				}else if(a->lon.coordinatevaluef < b->lat.coordinatevaluef){
					return 1;
				}
				else if(a->lon.coordinatevaluef == b->lat.coordinatevaluef){
				

				if(a->lat.coordinatevaluei > b->lon.coordinatevaluei ){
					return -1;
				}else if(a->lat.coordinatevaluei < b->lon.coordinatevaluei){
					return 1;
				}else if(a->lat.coordinatevaluei == b->lon.coordinatevaluei){//if both lat and lon equal, we check name lenght
						
				if(a->lat.coordinatevaluef > b->lon.coordinatevaluef){
					return -1;
				}else if(a->lat.coordinatevaluef < b->lon.coordinatevaluef){
					return 1;
				}
				else if(a->lat.coordinatevaluef == b->lon.coordinatevaluef){
										
						
						counta=0,
						countb=0;
						for(int i = 0; i < LEN; ++i){
						if(a->LocationName[i]<255 && a->LocationName[i]>0){
							++counta;
						}
						if(b->LocationName[i]<255 && b->LocationName[i]>0){
							++countb;
						}
					}
					if(counta < countb) return -1;
					else if(counta > countb) return 1;
					else return 0;
				}
				
				}

				}
			}
		}else{//if same left lon and same right lat

			if(a->lon.coordinatevaluei > b->lon.coordinatevaluei){
				return -1;
			}else if(a->lon.coordinatevaluei < b->lon.coordinatevaluei){
				return 1;
			}else if(a->lon.coordinatevaluei == b->lon.coordinatevaluei){//latitude equal, we check longitude

				if(a->lon.coordinatevaluef > b->lon.coordinatevaluef){
					return -1;
				}else if(a->lon.coordinatevaluef < b->lon.coordinatevaluef){
					return 1;
				}
				else if(a->lon.coordinatevaluef == b->lon.coordinatevaluef){
				

				if(a->lat.coordinatevaluei > b->lat.coordinatevaluei ){
					return -1;
				}else if(a->lat.coordinatevaluei < b->lat.coordinatevaluei){
					return 1;
				}else if(a->lat.coordinatevaluei == b->lat.coordinatevaluei){//if both lat and lon equal, we check name lenght
						
				if(a->lat.coordinatevaluef > b->lat.coordinatevaluef){
					return -1;
				}else if(a->lat.coordinatevaluef < b->lat.coordinatevaluef){
					return 1;
				}
				else if(a->lat.coordinatevaluef == b->lat.coordinatevaluef){
												
						counta=0,
						countb=0;
						for(int i = 0; i < LEN; ++i){
						if(a->LocationName[i]<255 && a->LocationName[i]>0){
							++counta;
						}
						if(b->LocationName[i]<255 && b->LocationName[i]>0){
							++countb;
						}
					}
					if(counta < countb) return -1;
					else if(counta > countb) return 1;
					else return 0;
				}
				}
			}
			}

		}

	}
	// if (amag < bmag)
	// 	return -1;
	// if (amag > bmag)
	// 	return 1;
	return 0;
}


//to have a function to achieve !~,~

static bool
geocoord_abs_cmp_internal_zone(GeoCoord * a, GeoCoord * b){

	int		zonea = 0,
			zoneb = 0;

	//Calculate the time zone
	if(a->lat.direction == 'N' || a->lat.direction == 'S'){//a left lat and right lon

		if(b->lat.direction == 'N' || b->lat.direction == 'S'){//and b also left lat and right lon
			//caluclate time zone an compare
				if(a->lon.direction=='W'){ //if a is West
					if(b->lon.direction=='W'){//Same direction
						//can calculate
						zonea = 0;
						zoneb = 0;

						zonea = (a->lon.coordinatevaluei)/15;
						zoneb = (b->lon.coordinatevaluei)/15;
						if(zonea == zoneb) return true;
						else return false;
					}else{//a b not same direction
						return false;
						
					}
				}else{//if a is East
					if(b->lon.direction=='W'){//not same direction
						return false;
					}else{//same direction, both east
						zonea = 0;
						zoneb = 0;

						zonea = a->lon.coordinatevaluei/15;
						zoneb = b->lon.coordinatevaluei/15;

						if(zonea == zoneb) return true;
						else return false;						
					}
				}

		}else{//b left is lon

				if(a->lon.direction=='W'){
					if(b->lat.direction == 'W')//same direction
					{
						zonea = 0;
						zoneb = 0;

						zonea = a->lon.coordinatevaluei/15;
						zoneb = b->lat.coordinatevaluei/15;

						if(zonea == zoneb) return true;
						else return false;		
					}else{
						return false;
					}


				}else{// a is East

					if(b->lat.direction == 'W')//not same direction
					{
						return false;
					}else{//both East
						zonea = 0;
						zoneb = 0;

						zonea = a->lon.coordinatevaluei/15;
						zoneb = b->lat.coordinatevaluei/15;

						if(zonea == zoneb) return true;
						else return false;		
					}

				}

		}

	}else{ // if a left lon
		if(b->lat.direction == 'N' || b->lat.direction == 'S'){//but b left still lat

			if(a->lat.direction == 'W'){
				if(b->lon.direction=='W'){//same direction
						zonea = 0;
						zoneb = 0;

						zonea = a->lat.coordinatevaluei/15;
						zoneb = b->lon.coordinatevaluei/15;

						if(zonea == zoneb) return true;
						else return false;		
				}else{
					return false;
				}

			}else{// a is East
				if(b->lon.direction=='W'){//not same direction
					return false;
				}else{
						zonea = 0;
						zoneb = 0;

						zonea = a->lat.coordinatevaluei/15;
						zoneb = b->lon.coordinatevaluei/15;

						if(zonea == zoneb) return true;
						else return false;							
				}
			}
			
		}else{//b also left lon

			if(a->lat.direction == 'W'){
				if(b->lat.direction == 'W'){
						zonea = 0;
						zoneb = 0;

						zonea = a->lat.coordinatevaluei/15;
						zoneb = b->lat.coordinatevaluei/15;

						if(zonea == zoneb) return true;
						else return false;		
				}else{
					return false;
				}

			}else{//a East
				if(b->lat.direction == 'W'){//a east b west
					return false;
				}else{//same
						zonea = 0;
						zoneb = 0;

						zonea = a->lat.coordinatevaluei/15;
						zoneb = b->lat.coordinatevaluei/15;

						if(zonea == zoneb) return true;
						else return false;		
				}
			}
		}
	}

	return false;
}



//Covertfuntion
PG_FUNCTION_INFO_V1(convert2dms);

Datum
convert2dms(PG_FUNCTION_ARGS){

	GeoCoord    *geoCoord = (GeoCoord *) PG_GETARG_POINTER(0);

	char	   	*result,
				// *res_latD,
				// *res_lonD,
				// *res_latM,
				// *res_lonM,
				*res_latS,
				*res_lonS;
	int			D_NS, D_WE,
				M_NS, M_WE,
				S_NS, S_WE;


	D_NS = floor(geoCoord->lat.coordinatevaluei);
	M_NS = floor(60 *((double)geoCoord->lat.coordinatevaluef/10000));
	S_NS = floor(3600 * ((double)geoCoord->lat.coordinatevaluef/10000) - 60 * M_NS);

	D_WE = floor(geoCoord->lon.coordinatevaluei);
	M_WE = floor(60 *((double)geoCoord->lon.coordinatevaluef/10000));
	S_WE = floor(3600 * ((double)geoCoord->lon.coordinatevaluef/10000) - 60 * M_WE);


	
	if(M_NS!=0){
		if(S_NS!=0){
			res_latS = psprintf("%d°%d'%d\"", D_NS, M_NS, S_NS);
		}
		else{
			res_latS = psprintf("%d°%d'", D_NS ,M_NS);
		}

	}
	else{
		if(S_NS!=0){
			res_latS = psprintf("%d°%d\"", D_NS, S_NS);
		}
		else{
			res_latS = psprintf("%d°", D_NS);
		}
	}


	if(M_WE!=0){
		if(S_WE!=0){
			res_lonS = psprintf("%d°%d'%d\"", D_WE, M_WE, S_WE);
		}
		else{
			res_lonS = psprintf("%d°%d'", D_WE ,M_WE);
		}

	}
	else{
		if(S_WE!=0){
			res_lonS = psprintf("%d°%d\"", D_WE, S_WE);
		}
		else{
			res_lonS = psprintf("%d°", D_WE);
		}
	}

	
	
	result = psprintf("%s, %s%c, %s%c", geoCoord->LocationName, res_latS, geoCoord->lat.direction, res_lonS, geoCoord->lon.direction);

	PG_RETURN_CSTRING(result);
}




PG_FUNCTION_INFO_V1(geocoord_abs_lt);

Datum
geocoord_abs_lt(PG_FUNCTION_ARGS)
{
	GeoCoord    *a = (GeoCoord *) PG_GETARG_POINTER(0);
	GeoCoord    *b = (GeoCoord *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(geocoord_abs_cmp_internal(a, b) < 0);
}

PG_FUNCTION_INFO_V1(geocoord_abs_le);

Datum
geocoord_abs_le(PG_FUNCTION_ARGS)
{
	GeoCoord    *a = (GeoCoord *) PG_GETARG_POINTER(0);
	GeoCoord    *b = (GeoCoord *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(geocoord_abs_cmp_internal(a, b) <= 0);
}

PG_FUNCTION_INFO_V1(geocoord_abs_eq);

Datum
geocoord_abs_eq(PG_FUNCTION_ARGS)
{
	GeoCoord    *a = (GeoCoord *) PG_GETARG_POINTER(0);
	GeoCoord    *b = (GeoCoord *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(geocoord_abs_cmp_internal(a, b) == 0);
}


PG_FUNCTION_INFO_V1(geocoord_abs_neq);

Datum
geocoord_abs_neq(PG_FUNCTION_ARGS)
{
	GeoCoord    *a = (GeoCoord *) PG_GETARG_POINTER(0);
	GeoCoord    *b = (GeoCoord *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(geocoord_abs_cmp_internal(a, b) != 0);
}

PG_FUNCTION_INFO_V1(geocoord_abs_ge);

Datum
geocoord_abs_ge(PG_FUNCTION_ARGS)
{
	GeoCoord    *a = (GeoCoord *) PG_GETARG_POINTER(0);
	GeoCoord    *b = (GeoCoord *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(geocoord_abs_cmp_internal(a, b) >= 0);
}

PG_FUNCTION_INFO_V1(geocoord_abs_gt);

Datum
geocoord_abs_gt(PG_FUNCTION_ARGS)
{
	GeoCoord    *a = (GeoCoord *) PG_GETARG_POINTER(0);
	GeoCoord    *b = (GeoCoord *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(geocoord_abs_cmp_internal(a, b) > 0);
}

PG_FUNCTION_INFO_V1(geocoord_abs_cmp);

Datum
geocoord_abs_cmp(PG_FUNCTION_ARGS)
{
	GeoCoord    *a = (GeoCoord *) PG_GETARG_POINTER(0);
	GeoCoord    *b = (GeoCoord *) PG_GETARG_POINTER(1);

	PG_RETURN_INT32(geocoord_abs_cmp_internal(a, b));
}

//hash
PG_FUNCTION_INFO_V1(geocoord_abs_cmp_);

Datum
geocoord_abs_cmp_(PG_FUNCTION_ARGS)
{
	GeoCoord    *a = (GeoCoord *) PG_GETARG_POINTER(0);
	GeoCoord    *b = (GeoCoord *) PG_GETARG_POINTER(1);

	PG_RETURN_INT32(geocoord_abs_cmp_internal(a, b));
}


PG_FUNCTION_INFO_V1(geocoord_hash1);

Datum
geocoord_hash1(PG_FUNCTION_ARGS)
{
	//GeoCoord    *a = (GeoCoord *) PG_GETARG_POINTER(0);

	PG_RETURN_INT32(0);
}

// PG_FUNCTION_INFO_V1(geocoord_hash);

// Datum
// geocoord_hash(PG_FUNCTION_ARGS)
// {
// 	text    *a = (text *) PG_GETARG_POINTER(0);
// 	text    *b = (text *) PG_GETARG_POINTER(1);

// 	PG_RETURN_INT32(a == b);
// }

// PG_FUNCTION_INFO_V1(geocoord_abs_eq_h);

// Datum
// geocoord_abs_eq_h(PG_FUNCTION_ARGS)
// {
// 	GeoCoord    *a = (GeoCoord *) PG_GETARG_POINTER(0);
// 	GeoCoord    *b = (GeoCoord *) PG_GETARG_POINTER(1);

// 	PG_RETURN_BOOL(geocoord_abs_cmp_internal(a, b) == 0);
// }

// PG_FUNCTION_INFO_V1(geocoord_hash3);

// Datum
// geocoord_hash3(PG_FUNCTION_ARGS)
// {
// 	GeoCoord    *a = (GeoCoord *) PG_GETARG_POINTER(0);
// 	GeoCoord    *b = (GeoCoord *) PG_GETARG_POINTER(1);

// 	PG_RETURN_BOOL(geocoord_abs_cmp_internal(a, b)==0);
// }


//!~ and ~
PG_FUNCTION_INFO_V1(geocoord_abs_samezone);

Datum
geocoord_abs_samezone(PG_FUNCTION_ARGS)
{
	GeoCoord    *a = (GeoCoord *) PG_GETARG_POINTER(0);
	GeoCoord    *b = (GeoCoord *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(geocoord_abs_cmp_internal_zone(a, b));
}

PG_FUNCTION_INFO_V1(geocoord_abs_notsamezone);

Datum
geocoord_abs_notsamezone(PG_FUNCTION_ARGS)
{
	GeoCoord    *a = (GeoCoord *) PG_GETARG_POINTER(0);
	GeoCoord    *b = (GeoCoord *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(!geocoord_abs_cmp_internal_zone(a, b));
}

