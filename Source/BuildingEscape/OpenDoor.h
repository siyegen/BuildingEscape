

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void OpenDoor();
	void CloseDoor();

	float GetTotalMassOfActorsOnPlate();

private:
	UPROPERTY(EditAnywhere)
	float MaxOpenAngle = 70.f;
	UPROPERTY(EditAnywhere)
	float OpenByValue = 2.f;
	UPROPERTY(EditAnywhere)
	float OpenMassKG  = 30.f;

	bool IsOpening = false;
	bool IsClosing = false;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.f;

	float LastDoorOpenTime;

	AActor* Owner = nullptr;

};
