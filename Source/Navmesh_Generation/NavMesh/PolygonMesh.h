// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Contour.h"
#include "PolygonMesh.generated.h"

#define NULL_INDEX -1

class AOpenHeightfield;

USTRUCT()
struct FContourData
{
	GENERATED_USTRUCT_BODY()

	//Coordinates of the vertices belonging to the contour
	TArray<FVector> Vertices;

	//Internal region ID to indicate the region the vertex belongs to
	int RegionID = 0;
};

USTRUCT()
struct FTriangleData
{
	GENERATED_USTRUCT_BODY()

	//Vertex inside inside the raw contour, useful for looping while creating the simplfied contour
	int Index = 0;

	bool IsTriangleCenterIndex = false;
};


UCLASS()
class NAVMESH_GENERATION_API APolygonMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APolygonMesh();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Initialize the base data of the polygon mesh using the one retrieved from the Height and Open fields
	void Init(const AOpenHeightfield* OpenHF);

	//Split the contour vertices based on the region they belong to and return the size of the biggest one
	int SplitContourDataByRegion(const AContour* Contour);

	//Generate the polygon mesh from the contour data provided
	void GeneratePolygonMesh(const AContour* Contour);

	//Attempt to triangluate a polygon based on the vertex and indices data provided
	//Return the total number of triangles generate, negative number if the generation fails
	int Triangulate(TArray<FVector>& Vertices, TArray<FTriangleData>& Indices, TArray<int>& Triangles);

	//Determine if vertex B of a polygon at vertex A lies within the internal angle
	//If not it means that the possible partition to subdvide the polygon is external to the polygon 
	bool LocatedInInternalAngle(int IndexA, int IndexB, TArray<FVector>& Vertices, TArray<FTriangleData>& Indices);

	//Check if the possible partition considered is intersecting one of the pre-existing edge of the polygon
	bool InvalidEdgeIntersection(int IndexA, int IndexB, TArray<FVector>& Vertices, TArray<FTriangleData>& Indices);

	//Use the sum of the condition values retrieved from the LocatedInInternalAngle and InvalidEdgeIntersection methods to determine if a partition 
	//can be considered or not for triangulation
	bool IsValidPartition(int IndexA, int IndexB, TArray<FVector>& Vertices, TArray<FTriangleData>& Indices);

	//Return the double of the signed area (the area of a triangle, if the vertices are listed counterclockwise
	//Or the negative of that area, if the vertices are listed clockwise) of the triangle
	float GetDoubleSignedArea(const FVector A, const FVector B, const FVector C);

	//True if the vertex to test is located to the left of the line
	bool IsVertexLeft(const FVector VertexToTest, const FVector LineStart, const FVector LineEnd);

	//True if the vertex to test is located to the left of the line or collinear with it
	bool IsVertexLeftOrCollinear(const FVector VertexToTest, const FVector LineStart, const FVector LineEnd);

	//True if the vertex to test is located to the right of the line
	bool IsVertexRight(const FVector VertexToTest, const FVector LineStart, const FVector LineEnd);

	//True if the vertex to test is located to the right of the line or collinear with it
	bool IsVertexRightOrCollinear(const FVector VertexToTest, const FVector LineStart, const FVector LineEnd);

	//Get the previous index of an array, making sure is wapped to the end if the index goes negative
	int GetPreviousArrayIndex(const int Index, const int ArraySize);

	//Get the next index of an array, making sure is wapped to the end if the index goes outside range
	int GetNextArrayIndex(const int Index, const int ArraySize);

	//Draw the triangles forming the polymesh
	void DrawDebugPolyMeshTriangles(const TArray<FVector>& Vertices, const TArray<int>& Triangles, const int NumberOfTriangles);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//Min coordinates of the heightfield derived from the bounds of the navmesh area
	FVector BoundMin;

	//Max coordinates of the heightfield derived from the bounds of the navmesh area
	FVector BoundMax;

	//Size of the single cells (voxels) in which the heightfiels is subdivided, the cells are squared
	float CellSize;

	//Height of the single cells (voxels) in which the heightfiels is subdivided
	float CellHeight;

	//Maximum number of vertices per polygon, clamped to be >= 3
	int MaxVertexPerPoly = 3;

	TArray<FContourData> ContoursData;
};