// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "KartPawn.generated.h"

USTRUCT()
struct FKartPawnMove
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Throttle;

	UPROPERTY()
	float SteeringThrow;

	UPROPERTY()
	float DeltaTime;

	UPROPERTY()
	float Time;
};

USTRUCT()
struct FKartPawnState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FKartPawnMove LastMove;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FTransform Transform;
};

UCLASS()
class PLAYKARTS_API AKartPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AKartPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	FVector GetAirResistance();
	FVector GetRollingResistance();

	FString GetEnumText(ENetRole InputRole);

	void ApplyRotation(float DeltaTime, float SteerThrow);
	void UpdateLocationFromVelocity(float DeltaTime);

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FKartPawnMove Move);

	void SimulateMove(FKartPawnMove Move);

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FKartPawnState ServerState;

	UFUNCTION()
	void OnRep_ServerState();

	FVector Velocity;

	// Mass of the vehicle in kg
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	// Force applied to car when the throttle is at max
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;

	// Min turning radius at full lock (metres)
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;

	// Drag coefficient: the higher it is, the more the drag
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;

	// RR coefficient: the higher it is, the more the rolling resistance
	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficient = 0.015;

	float Throttle;
	float SteeringThrow;
};
