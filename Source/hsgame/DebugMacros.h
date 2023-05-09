#pragma once

#include "DrawDebugHelpers.h" //built in header file that helps with debug sphere funtionality

#define DRAW_SPHERE(location) if(GetWorld()) DrawDebugSphere(GetWorld(), location, 25.f, 24, FColor::Blue, true);//FUNCTION MACRO LIKE IN BP's, makes things easier and less redundant
#define DRAW_SPHERE_SingleFrame(location) if(GetWorld()) DrawDebugSphere(GetWorld(), location, 25.f, 24, FColor::Blue, false, -1.f); //making single frame debug tools to see movement through ticks and delta time (visualize movement)
#define DRAW_LINE(startLocation, endLocation) if(GetWorld()) DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Purple, true);
#define DRAW_LINE_SingleFrame(location)	if(GetWorld()) DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Purple, false, -1.f);
#define DRAW_POINT(location) if(GetWorld()) DrawDebugPoint(GetWorld(), location, 15, FColor::Black, true);
#define DRAW_POINT_SingleFrame(location) if(GetWorld()) DrawDebugPoint(GetWorld(), location, 15, FColor::Black, false, -1.f);
#define DRAW_BOX(location, color) if(GetWorld()) DrawDebugBox(GetWorld(), location, location, color, true);
#define DRAW_VECTOR(startLocation, endLocation) if(GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Purple, true); \
		DrawDebugPoint(GetWorld(), endLocation, 15, FColor::Black, true); \
	}
#define DRAW_VECTOR_SingleFrame(startLocation, endLocation) if(GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Purple, false, -1.f); \
		DrawDebugPoint(GetWorld(), endLocation, 15, FColor::Black, false, -1.f); \
	}

//use backslash to expand macro to diff line, DRAW_VECTOR makes use of both DRAW_LINE and DRAW_POINT (very useful)

//put vars you want to access in different classes in .h files so they can be easily accessed

