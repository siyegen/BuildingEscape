

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("Missing pressure plate"));
	}
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (TickType != ELevelTick::LEVELTICK_PauseTick) {
		// Poll the Trigger Volume to see if should trigger
		//if (PressurePlate->IsOverlappingActor(ActorThatOpens)) {  // update to set flag
		if (GetTotalMassOfActorsOnPlate() > OpenMassKG) {
			IsOpening = true;
			LastDoorOpenTime = GetWorld()->GetTimeSeconds();
		}
		if (IsOpening && GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
			IsClosing = true, IsOpening = false;
		}
	}
	if (IsOpening) {
		OpenDoor();
	} else if (IsClosing) {
		CloseDoor();
	}
}

void UOpenDoor::OpenDoor() {
	float CurrentYaw = Owner->GetActorRotation().Yaw;
	if (CurrentYaw < MaxOpenAngle) {

		FRotator NewRotation = FRotator(0.0f, CurrentYaw + OpenByValue, 0.0f);

		Owner->SetActorRotation(NewRotation);
	}
}

void UOpenDoor::CloseDoor() {
	float CurrentYaw = Owner->GetActorRotation().Yaw;
	if (CurrentYaw > 0) {
		Owner->SetActorRotation(FRotator(0.f, CurrentYaw - OpenByValue, 0.f));
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	float TotalMass = 0.f;

	if (PressurePlate) {
		TArray<AActor*> OverlappingActors;
		PressurePlate->GetOverlappingActors(OverlappingActors);
		for (const auto& Actor : OverlappingActors) {
			TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
			UE_LOG(LogTemp, Warning, TEXT("%s on plate"), *Actor->GetName());
		}
		UE_LOG(LogTemp, Warning, TEXT("totalweight: %f"), TotalMass);
	}

	return TotalMass;
}

