// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZODH_NetActor.generated.h"

UCLASS()
class PNM_KFC_API AZODH_NetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZODH_NetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void FindOwner();

	UPROPERTY(EditAnywhere)
	float SearchDistance = 200.0f;
	
	UPROPERTY(Replicatedusing = OnRep_RotYaw)
	float RotYaw = 0.0f;

	UFUNCTION()
	void OnRep_RotYaw();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifeTimeProps) const;

	float CurTime = 0.0f;
	float lastTime = 0.0f;

	UPROPERTY()
	class UMaterialInstanceDynamic* Mat;
	
	UPROPERTY(ReplicatedUsing = OnRep_ChangeMatColor)
	FLinearColor MatColor;

	UFUNCTION()
	void OnRep_ChangeMatColor();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeColor(const FLinearColor InNewColor);

	UFUNCTION(Client, Reliable)
	void ClientRPC_ChangeColor(const FLinearColor newColor);

	UFUNCTION(Client, Unreliable)
	void MultiCastRPC_ChangeColor(const FLinearColor newColor);
};
