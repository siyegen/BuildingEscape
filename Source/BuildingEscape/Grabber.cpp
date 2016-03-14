

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
		FVector LineTraceEnd = GetLineTraceEnd();
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
			true
		);
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"));
	PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
	FHitResult LineHit;

	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("")), false, Owner);
	GetWorld()->LineTraceSingleByObjectType(
		LineHit,
		GetLineTraceStart(),
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	return LineHit;
}

FVector UGrabber::GetLineTraceStart() {
	FVector Location;
	FRotator Rotator;
	Player->GetPlayerViewPoint(Location, Rotator);
	return Location;
}

FVector UGrabber::GetLineTraceEnd() {
	FVector Location;
	FRotator Rotator;
	Player->GetPlayerViewPoint(Location, Rotator);
	return Location + Rotator.Vector() * Reach;;
}

