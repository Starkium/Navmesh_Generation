// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomNavigationData.h"
#include "NavMeshGenerator.h"
#include "NavmeshController.h"
#include "../Pathfinding/Pathfinding.h"

DEFINE_LOG_CATEGORY(CustomNavData);


ACustomNavigationData::ACustomNavigationData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		UE_LOG(CustomNavData, Log, TEXT("Start up with Flags"));

		FindPathImplementation = FindPath;
		FindHierarchicalPathImplementation = FindPath;
	}
}

void ACustomNavigationData::BeginPlay()
{
	Super::BeginPlay();
}

FPathFindingResult ACustomNavigationData::FindPath(const FNavAgentProperties& AgentProperties, const FPathFindingQuery& Query)
{
	//TO DO: Fix issue with pathfinding
	return FPathFindingResult(); // early return to avoid crash

	const ANavigationData* Self = Query.NavData.Get();
	const ACustomNavigationData* CustomNavmesh = (const ACustomNavigationData*)Self;
	check(CustomNavmesh != nullptr);
	UE_LOG(CustomNavData, Log, TEXT("Made it past weird double init navdata"));

	if (CustomNavmesh == nullptr)
	{

		UE_LOG(CustomNavData, Log, TEXT("CustomNavMesh was nullpts"));
		return FPathFindingResult();
		//return ENavigationQueryResult::Error;
	}
	UE_LOG(CustomNavData, Log, TEXT("Why are we marking an error here?"));
	FPathFindingResult Result(ENavigationQueryResult::Error);
	Result.Path = Query.PathInstanceToFill.IsValid() ? Query.PathInstanceToFill : Self->CreatePathInstance<FNavigationPath>(Query);

	UE_LOG(CustomNavData, Log, TEXT("About to get a path"));

	FNavigationPath* NavPath = Result.Path.Get();

	UE_LOG(CustomNavData, Log, TEXT("Got a path"));
	if (NavPath != nullptr)
	{
		UE_LOG(CustomNavData, Log, TEXT("Path wasn't null"));
		if ((Query.StartLocation - Query.EndLocation).IsNearlyZero())
		{
			UE_LOG(CustomNavData, Log, TEXT("We were nearly Zero"));
			Result.Path->GetPathPoints().Reset();
			Result.Path->GetPathPoints().Add(FNavPathPoint(Query.EndLocation));
			Result.Result = ENavigationQueryResult::Success;
		}
		else if (Query.QueryFilter.IsValid())
		{
			TArray<FVector> PathPoints;
			UE_LOG(CustomNavData, Log, TEXT("Before build path"));
			PathPoints = UPathfinding::BuildPath(Query.StartLocation, Query.EndLocation, CustomNavmesh);
			UE_LOG(CustomNavData, Log, TEXT("After Build path"));
			for (auto& Point : PathPoints)
			{
				UE_LOG(CustomNavData, Log, TEXT("Looping path points"));
				NavPath->GetPathPoints().Add(FNavPathPoint(Point));
			}
			UE_LOG(CustomNavData, Log, TEXT("prior mark ready"));
			NavPath->MarkReady();
			UE_LOG(CustomNavData, Log, TEXT("Post mark ready"));
			Result.Result = ENavigationQueryResult::Success;
		}
	}
	UE_LOG(CustomNavData, Log, TEXT("End Result"));
	return Result;
}

void ACustomNavigationData::ConditionalConstructGenerator()
{
    if (!NavDataGenerator.IsValid())
    {
        //Initialize the generator
        TSharedPtr<FNavMeshGenerator, ESPMode::ThreadSafe> NewGen(new FNavMeshGenerator());
        NewGen.Get()->SetNavmesh(this);
        NewGen.Get()->SetNavBounds();

		//Initialize the controller
		CreateNavmeshController();
		NavMeshController->SetNavGenerator(NewGen);
		NavMeshController->UpdateEditorPosition();
	

        //Construct the navmesh
        NewGen.Get()->GatherValidOverlappingGeometries();
        NewGen.Get()->GenerateNavmesh();

        NavDataGenerator = MoveTemp(NewGen);
    }
}

UPrimitiveComponent* ACustomNavigationData::ConstructRenderingComponent()
{
	return nullptr;
}

void ACustomNavigationData::CreateNavmeshController()
{
    //Make sure the controller is created only if it doesn't already exist
    if (!NavMeshController)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        NavMeshController = GetWorld()->SpawnActor<ANavMeshController>(FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), SpawnInfo);
    }
}

void ACustomNavigationData::SetResultingPoly(TArray<FPolygonData> NavPoly)
{
    ResultingPoly = NavPoly;
}
