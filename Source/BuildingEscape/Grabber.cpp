

#include "BuildingEscape.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	Player = GetWorld()->GetFirstPlayerController();
	SetupPhysicsComponent();
	SetupInputComponent();
}

void UGrabber::SetupPhysicsComponent() {
	PhysicsHandle = Owner->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("Missing P.handle on %s"), *Owner->GetName());
	}
}

void UGrabber::SetupInputComponent() {
	Input = Owner->FindComponentByClass<UInputComponent>();
	if (Input) {
		Input->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Missing Input on %s"), *Owner->GetName());
	}
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	if (PhysicsHandle->GrabbedComponent) {
		FVector Location;
		FRotator rotator;
		Player->GetPlayerViewPoint(Location, rotator);
		FVector LineTraceEnd = Location + rotator.Vector() * Reach;
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"));
	FHitResult Hit = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ToGrab = Hit.GetComponent();
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit) {
		PhysicsHandle->GrabComponent(
			Hit.GetComponent(),
			NAME_None,
			ToGrab->GetOwner()->GetActorLocation(),
			false
		);
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"));
	PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
	FHitResult LineHit;
	FVector Location;
	FRotator rotator;
	Player->GetPlayerViewPoint(Location, rotator);
	FVector LineTraceEnd = Location + rotator.Vector() * Reach;

	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("")), false, Owner);
	bool DidHit = GetWorld()->LineTraceSingleByObjectType(
		LineHit,
		Location,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
		);

	if (DidHit) {
		UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *LineHit.Actor->GetName());
	}
	return LineHit;
}

