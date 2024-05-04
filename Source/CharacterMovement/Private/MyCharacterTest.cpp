// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterTest.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
AMyCharacterTest::AMyCharacterTest()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Jumping = false;

}

// Called when the game starts or when spawned
void AMyCharacterTest::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashContext, 0);
		}
	}
}

void AMyCharacterTest::Move(const FInputActionValue& Value)
{
	
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y * Speed * CrawlSpeed);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X * Speed * CrawlSpeed * DodgeSpeed);
	

	//const FVector Forward = GetActorForwardVector();
	//AddMovementInput(Forward, MovementVector.Y);
	//
	//const FVector Rigth = GetActorRightVector();
	//AddMovementInput(Rigth, MovementVector.X);

}

void AMyCharacterTest::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void AMyCharacterTest::Jump()
{
	if (!Crawling) 
	{
		Jumping = true;
		Super::Jump();
	}
}

void AMyCharacterTest::Crawl()
{
	Crawling = true;
	CrawlSpeed = 0.25f;
}


void AMyCharacterTest::StopCrawl()
{
	Crawling = false;
	CrawlSpeed = 1.f;
}

void AMyCharacterTest::Dodge()
{
	Dodging = true;
}

void AMyCharacterTest::StopDodging()
{
	Dodging = false;
}

// Called every frame
void AMyCharacterTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyCharacterTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AMyCharacterTest::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacterTest::Look);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMyCharacterTest::Jump);

		EnhancedInputComponent->BindAction(CrawlAction, ETriggerEvent::Started, this, &AMyCharacterTest::Crawl);
		EnhancedInputComponent->BindAction(CrawlAction, ETriggerEvent::Completed, this, &AMyCharacterTest::StopCrawl);
		EnhancedInputComponent->BindAction(CrawlAction, ETriggerEvent::Canceled, this, &AMyCharacterTest::StopCrawl);

		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &AMyCharacterTest::Dodge);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Completed, this, &AMyCharacterTest::StopDodging);

	}

}


