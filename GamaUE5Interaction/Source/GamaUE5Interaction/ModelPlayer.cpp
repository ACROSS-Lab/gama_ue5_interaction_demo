// Fill out your copyright notice in the Description page of Project Settings.

#include "ModelPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "ObjectHandler.h"

// Sets default values
AModelPlayer::AModelPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create our components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	StaticMeshComp = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	//Attach our components
	StaticMeshComp->SetupAttachment(RootComponent);
	SpringArmComp->SetupAttachment(StaticMeshComp);
	CameraComp->SetupAttachment(SpringArmComp,USpringArmComponent::SocketName);

	//Assign SpringArm class variables.
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 3.0f;
	
	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AModelPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AModelPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Zoom in if ZoomIn button is down, zoom back out if it's not
	{
		if (bZoomingIn)
		{
			ZoomFactor += DeltaTime / 0.5f;         //Zoom in over half a second
		}
		else
		{
			ZoomFactor -= DeltaTime / 0.25f;        //Zoom out over a quarter of a second
		}
		ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);
		//Blend our camera's FOV and our SpringArm's length based on ZoomFactor
		CameraComp->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFactor);
		SpringArmComp->TargetArmLength = FMath::Lerp<float>(400.0f, 300.0f, ZoomFactor);
	}

	//Rotate our actor's yaw, which will turn our camera because we're attached to it
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CameraInput.X;
		SetActorRotation(NewRotation);
	}

	//Rotate our camera's pitch, but limit it so we're always looking downward
	{
		FRotator NewRotation = SpringArmComp->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, -15.0f);
		SpringArmComp->SetWorldRotation(NewRotation);
	}

	//Handle movement based on our "MoveX" and "MoveY" axes
	{
		if (!MovementInput.IsZero())
		{
			//Scale our movement input axis values by 100 units per second
			MovementInput = MovementInput.GetSafeNormal() * 100.0f;
			FVector NewLocation = GetActorLocation();
			NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime;
			NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime;
			SetActorLocation(NewLocation);
		}
	}
}

// Called to bind functionality to input
void AModelPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Hook up events for "ZoomIn"
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &AModelPlayer::ZoomIn);
	InputComponent->BindAction("ZoomIn", IE_Released, this, &AModelPlayer::ZoomOut);

	//Hook up events for Click
	//InputComponent->BindAction("Click", IE_Pressed, this, &AModelPlayer::Click);


	//Hook up every-frame handling for our four axes
	InputComponent->BindAxis("VerticalMove", this, &AModelPlayer::Move_XAxis);
	InputComponent->BindAxis("HorizontalMove", this, &AModelPlayer::Move_YAxis);
	InputComponent->BindAxis("CameraPitch", this, &AModelPlayer::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &AModelPlayer::YawCamera);
}

void AModelPlayer::Move_XAxis(float AxisValue)
{
	MovementInput.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void AModelPlayer::Move_YAxis(float AxisValue)
{
	MovementInput.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void AModelPlayer::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void AModelPlayer::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void AModelPlayer::ZoomIn()
{
	bZoomingIn = true;
}

void AModelPlayer::ZoomOut()
{
	bZoomingIn = false;
}



