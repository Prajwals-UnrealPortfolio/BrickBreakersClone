#include "LevelEditor/SLevelEditorTool.h"

#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"
#include "SlateOptMacros.h"
#include "Framework/Notifications/NotificationManager.h"
#include "IContentBrowserSingleton.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "BrickBreakersClone/Public/LevelData/BrickLevelData.h"
#include "UObject/SavePackage.h"

#define LOCTEXT_NAMESPACE "SLevelEditorTool"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SLevelEditorTool::Construct(const FArguments& InArgs)
{
	// Initialize grid
	GridBricks.SetNumZeroed(GridRows * GridCols);

	BricksColors.Add(FBrickColor(EMPTY, FLinearColor::Gray));
	BricksColors.Add(FBrickColor(NORMAL, FLinearColor::Green));
	BricksColors.Add(FBrickColor(CHAINED, FLinearColor::Yellow));
	BricksColors.Add(FBrickColor(FIRE, FLinearColor::Red));
	BricksColors.Add(FBrickColor(MULTIPLE_BALL, FLinearColor::Blue));
	BricksColors.Add(FBrickColor(SIZE_INCREASE, FLinearColor::White));
	BricksColors.Add(FBrickColor(SIZE_DECREASE, FLinearColor::White));

	CurrentBrickType = NORMAL;

	// Create main UI structure
	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(SHorizontalBox)

			// Rows Control
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Rows:"))
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5)
			[
				SAssignNew(RowSpinBox, SSpinBox<int32>)
				.MinValue(1)
				.MaxValue(100)
				.Value(GridRows)
				.OnValueChanged_Lambda([this](int32 NewRows)
				{
					GridRows = NewRows;
					OnGridSizeChanged();
				})
			]

			// Columns Control
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Columns:"))
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5)
			[
				SAssignNew(ColSpinBox, SSpinBox<int32>)
				.MinValue(1)
				.MaxValue(100)
				.Value(GridCols)
				.OnValueChanged_Lambda([this](int32 NewCols)
				{
					GridCols = NewCols;
					OnGridSizeChanged();
				})
			]
		]
		
		// Control buttons
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5)
			[
				SNew(SButton)
				.Text(FText::FromString("Save Level"))
				.OnClicked(this, &SLevelEditorTool::OnSaveLevel)
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5)
			[
				SNew(SButton)
				.Text(FText::FromString("Load Level"))
				.OnClicked(this, &SLevelEditorTool::OnLoadLevel)
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5)
			[
				SNew(SButton)
				.Text(FText::FromString("Clear Grid"))
				.OnClicked(this, &SLevelEditorTool::OnClearGrid)
			]
		]

		//Brick Color
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(SHorizontalBox)
		
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5)
			[
				SNew(SButton)
				.Text(FText::FromString("NORMAL"))
				.ForegroundColor(GetColorByBrickType(NORMAL))
				.OnClicked(this, &SLevelEditorTool::OnBrickTypeClicked, NORMAL)
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5)
			[
				SNew(SButton)
				.Text(FText::FromString("CHAINED"))
				.ForegroundColor(GetColorByBrickType(CHAINED))
				.OnClicked(this, &SLevelEditorTool::OnBrickTypeClicked, CHAINED)
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5)
			[
				SNew(SButton)
				.Text(FText::FromString("FIRE"))
				.ForegroundColor(GetColorByBrickType(FIRE))
				.OnClicked(this, &SLevelEditorTool::OnBrickTypeClicked, FIRE)
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5)
			[
				SNew(SButton)
				.Text(FText::FromString("MULTIPLE_BALL"))
				.ForegroundColor(GetColorByBrickType(MULTIPLE_BALL))
				.OnClicked(this, &SLevelEditorTool::OnBrickTypeClicked, MULTIPLE_BALL)
			]
			
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5)
			[
				SNew(SButton)
				.Text(FText::FromString("SIZE_INCREASE"))
				.ForegroundColor(GetColorByBrickType(SIZE_INCREASE))
				.OnClicked(this, &SLevelEditorTool::OnBrickTypeClicked, SIZE_INCREASE)
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5)
			[
				SNew(SButton)
				.Text(FText::FromString("SIZE_DECREASE"))
				.ForegroundColor(GetColorByBrickType(SIZE_DECREASE))
				.OnClicked(this, &SLevelEditorTool::OnBrickTypeClicked, SIZE_DECREASE)
			]
		]

		// Grid visualization
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.Padding(5)
		[
			SAssignNew(GridPanel, SGridPanel)
		]
	];
	
	ClearDataInGrids();
	UpdateGrid();
}

FReply SLevelEditorTool::OnBrickTypeClicked(EBRICK_TYPE BrickType)
{
	SetBrickType(BrickType);
	return FReply::Handled();
}

FReply SLevelEditorTool::OnSaveLevel()
{
	OnOpenSaveDialog();
	return FReply::Handled();
}

void SLevelEditorTool::OnOpenSaveDialog()
{
	TSharedRef<SWindow> SaveDialogWindow = SNew(SWindow)
		.Title(LOCTEXT("SaveDialogTitle", "Save Level As"))
		.ClientSize(FVector2D(400, 150))
		.SupportsMaximize(false)
		.SupportsMinimize(false);

	TSharedPtr<SEditableTextBox> NameTextBox;

	SaveDialogWindow->SetContent(
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("SaveDialogPrompt", "Enter a name for the level:"))
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SAssignNew(NameTextBox, SEditableTextBox)
			.MinDesiredWidth(300.0f)
			.Text(FText::FromString("NewLevel"))
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(SButton)
			.Text(LOCTEXT("SaveDialogOKButton", "Save"))
			.OnClicked_Lambda([this, SaveDialogWindow, NameTextBox]()
			{
				FString LevelName = NameTextBox->GetText().ToString();
				if (!LevelName.IsEmpty())
				{
					SaveLevelAs(LevelName);
					FSlateApplication::Get().RequestDestroyWindow(SaveDialogWindow);
				}
				return FReply::Handled();
			})
		]
	);

	FSlateApplication::Get().AddModalWindow(SaveDialogWindow, nullptr);
}

FReply SLevelEditorTool::OnLoadLevel()
{
	OnOpenAssetPicker();
	return FReply::Handled();
}

FReply SLevelEditorTool::OnClearGrid()
{
	ClearDataInGrids();
	UpdateGrid();
	UE_LOG(LogTemp, Log, TEXT("Grid cleared."));
	return FReply::Handled();
}

void SLevelEditorTool::UpdateGrid()
{
	GridPanel->ClearChildren();
	
	// Rebuild the grid visualization dynamically
	for (int32 Row = 0; Row < GridRows; ++Row)
	{
		for (int32 Col = 0; Col < GridCols; ++Col)
		{
			int32 Index = GetGridIndex(Row, Col);

			GridPanel->AddSlot(Col, Row)
			.Padding(5.0f)
			[
				SNew(SBorder)
				.BorderBackgroundColor( GetColorByBrickType(GridBricks[Index]->BrickState))
				.ForegroundColor(GetColorByBrickType(GridBricks[Index]->BrickState))
				.OnMouseButtonDown_Lambda([this, Row, Col](const FGeometry&, const FPointerEvent&) -> FReply
				{
					if (GridBricks[GetGridIndex(Row, Col)]->BrickState != EMPTY)
					{
						RemoveBrickFromGrid(Row, Col);
					}
					else
					{
						AddBrickToGrid(Row, Col, CurrentBrickType);
					}
					return FReply::Handled();
				})
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromString(GridBricks[Index]->BrickState != EMPTY ? EBrickEnumType->GetNameStringByIndex(static_cast<uint32>(GridBricks[Index]->BrickState)) : TEXT("     ")))
				]
			];
		}
	}
}

void SLevelEditorTool::SaveDataAsset(UBrickLevelData* LevelData, const FString& AssetName)
{
	FString PackagePath = FString::Printf(TEXT("/Game/BrickBreakerLevels/%s"), *AssetName);
	UPackage* Package = CreatePackage(*PackagePath);
	const FString PackageName = Package->GetName();
	const FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());

	FSoftObjectPath SoftObjectPath(PackagePath);
	
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	FAssetData ExistingAssetData = AssetRegistryModule.Get().GetAssetByObjectPath(SoftObjectPath);

	if (ExistingAssetData.IsValid())
	{
		EAppReturnType::Type UserResponse = FMessageDialog::Open(
			EAppMsgType::YesNo, 
			FText::Format(
				NSLOCTEXT("LevelEditor", "OverwriteLevel", "Asset '{0}' already exists. Do you want to overwrite it?"), 
				FText::FromString(AssetName)
			)
		);

		// If the user chooses No, abort the process
		if (UserResponse == EAppReturnType::No)
		{
			ShowNotification(TEXT("Operation cancelled. Asset will not be overwritten."), SNotificationItem::CS_Fail);
			return;
		}

		FAssetToolsModule& AssetTools = FAssetToolsModule::GetModule();
		UObject* NewAsset = AssetTools.Get().CreateAsset(AssetName, "/Game/BrickBreakerLevels", UBrickLevelData::StaticClass(), nullptr);
	
		UBrickLevelData* Asset = Cast<UBrickLevelData>(NewAsset);
		if (!Asset)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create asset"));
			ShowNotification(TEXT("Failed to create asset."), SNotificationItem::CS_Fail);
			return;
		}
	
		Asset->Bricks = LevelData->Bricks;
		Asset->GridSize = LevelData->GridSize;
		Asset->CellSize = LevelData->CellSize;
	
		Package->MarkPackageDirty();

		FSavePackageArgs SaveArgs;
		SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	
		FSavePackageResultStruct PackageResult = UPackage::Save(Package, Asset, *PackageFileName, SaveArgs);

		if (!PackageResult.IsSuccessful())
		{
			UE_LOG(LogTemp, Error, TEXT("Package '%s' wasn't saved!"), *PackageName)
			ShowNotification(TEXT("Package wasn't saved!"), SNotificationItem::CS_Fail);
		}

		ShowNotification(FString::Printf(TEXT("Level saved successfully: %s"), *AssetName), SNotificationItem::CS_Success);
		UE_LOG(LogTemp, Warning, TEXT("Package '%s' was successfully saved"), *PackageName)
		Package->FullyLoad();
	}
	
}

bool SLevelEditorTool::LoadDataAsset(const FString& AssetPath)
{
	if (UBrickLevelData* LoadedData = Cast<UBrickLevelData>(StaticLoadObject(UBrickLevelData::StaticClass(), nullptr, *AssetPath)))
	{
		CurrentLevelData = LoadedData;
		GridRows = CurrentLevelData->GridSize.X;
		GridCols = CurrentLevelData->GridSize.Y;
		CellSize = CurrentLevelData->CellSize;
		GridBricks.SetNumZeroed(GridRows * GridCols);
		for (const FBrickInfo& Position : CurrentLevelData->Bricks)
		{
			int32 Row = FMath::FloorToInt(Position.Row / CellSize);
			int32 Col = FMath::FloorToInt(Position.Column / CellSize);
			GridBricks[GetGridIndex(Row, Col)] = Position;
		}
		return true;
	}
	return false;
}

void SLevelEditorTool::OnAssetSelected(const FAssetData& SelectedAsset)
{
	if (SelectedAsset.IsValid())
	{
		if(LoadDataAsset(SelectedAsset.GetAsset()->GetPathName()))
		{
			UpdateGrid();
			ShowNotification(FString::Printf(TEXT("Level loaded successfully: %s"), *SelectedAsset.AssetName.ToString()), SNotificationItem::CS_Success);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Selected asset is not a valid UBrickLevelData."));
			ShowNotification(TEXT("Failed to load the level."), SNotificationItem::CS_Fail);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid asset selected."));
		ShowNotification(TEXT("Invalid asset selected."), SNotificationItem::CS_Fail);
	}
}

void SLevelEditorTool::AddBrickToGrid(int32 Row, int32 Column, EBRICK_TYPE BrickState)
{
	int32 Index = GetGridIndex(Row, Column);
	if (GridBricks.IsValidIndex(Index) && !GridBricks[Index]->BrickState != EMPTY)
	{
		GridBricks[Index] = FBrickInfo(Row * CellSize, Column * CellSize, BrickState);
		UpdateGrid();
	}
}

void SLevelEditorTool::RemoveBrickFromGrid(int32 Row, int32 Column)
{
	int32 Index = GetGridIndex(Row, Column);
	if (GridBricks.IsValidIndex(Index) && GridBricks[Index]->BrickState != EMPTY)
	{
		GridBricks[Index]->BrickState = EMPTY;
		UpdateGrid();
	}
}

void SLevelEditorTool::SaveLevelAs(const FString& LevelName)
{
	if (!IsAssetNameValid(LevelName))
	{
		return;
	}
	
	if (!CurrentLevelData)
	{
		// Create a new level data asset
		CurrentLevelData = NewObject<UBrickLevelData>(GetTransientPackage(), UBrickLevelData::StaticClass());
	}

	// Collect brick information
	CurrentLevelData->Bricks.Empty();
	
	for (const auto& Brick : GridBricks)
	{
		FBrickInfo BrickInfo;
		BrickInfo.Row = Brick->Row;
		BrickInfo.Column = Brick->Column;
		BrickInfo.BrickState = Brick->BrickState;
		CurrentLevelData->Bricks.Add(BrickInfo);
	}

	CurrentLevelData->GridSize = FVector2D(GridRows, GridCols);
	CurrentLevelData->CellSize = CellSize;

	// Save asset
	SaveDataAsset(CurrentLevelData, LevelName);
	UE_LOG(LogTemp, Log, TEXT("Level saved successfully as %s!"), *LevelName);
	
}

bool SLevelEditorTool::IsAssetNameValid(const FString& AssetName) const
{
	if (AssetName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Asset name cannot be empty."));
		return false;
	}

	// Check if an asset with the same name already exists
	FString AssetPath = FString::Printf(TEXT("/Game/BrickBreakerLevels/%s"), *AssetName);
	if (FPackageName::DoesPackageExist(AssetPath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Asset with name '%s' already exists."), *AssetName);
		return false;
	}

	return true;
}

void SLevelEditorTool::ShowNotification(const FString& Message, SNotificationItem::ECompletionState State)
{
	FNotificationInfo Info(FText::FromString(Message));
	Info.bFireAndForget = true;
	Info.FadeOutDuration = 1.0f;
	Info.bUseThrobber = false;
	Info.ExpireDuration = 3.0f;
	
	TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(Info);
	if (Notification.IsValid())
	{
		Notification->SetCompletionState(State);
	}
}

void SLevelEditorTool::OnGridSizeChanged()
{
	TArray<TOptional<FBrickInfo>> NewGridBricks;
	NewGridBricks.SetNumZeroed(GridRows * GridCols);

	// Copy existing bricks into the new array
	for (int32 Row = 0; Row < GridRows; ++Row)
	{
		for (int32 Col = 0; Col < GridCols; ++Col)
		{
			int32 OldIndex = GetGridIndex(Row, Col);
			int32 NewIndex = Row * GridCols + Col;

			if (OldIndex < GridBricks.Num())
			{
				NewGridBricks[NewIndex] = GridBricks[OldIndex];
			}
			else
			{
				NewGridBricks[NewIndex] = FBrickInfo(Row * CellSize, Col * CellSize, EMPTY);
			}
		}
	}

	GridBricks = MoveTemp(NewGridBricks);
	
	UpdateGrid();
}

void SLevelEditorTool::ClearDataInGrids()
{
	for (int32 Row = 0; Row < GridRows; ++Row)
	{
		for (int32 Col = 0; Col < GridCols; ++Col)
		{
			int32 Index = GetGridIndex(Row, Col);
			GridBricks[Index] = FBrickInfo(Row * CellSize, Col * CellSize, EMPTY);
		}
	}
}

FLinearColor SLevelEditorTool::GetColorByBrickType(EBRICK_TYPE BrickState)
{
	FBrickColor BrickColor = BricksColors[0];
	for (int index = 0; index<BricksColors.Num();index++)
	{
		if(BricksColors[index].BrickType == BrickState)
		{
			BrickColor = BricksColors[index];
			return BrickColor.BrickColor;
		}
	}
	return BrickColor.BrickColor;
}

void SLevelEditorTool::SetBrickType(EBRICK_TYPE BrickType)
{
	CurrentBrickType = BrickType;
}

void SLevelEditorTool::OnOpenAssetPicker()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	FAssetPickerConfig AssetPickerConfig;

	TSharedRef<SWindow> AssetPickerWindow = SNew(SWindow)
		.Title(LOCTEXT("AssetPickerWindowTitle", "Select a Level"))
		.ClientSize(FVector2D(800, 600))
		.SupportsMaximize(false)
		.SupportsMinimize(false);
	
	AssetPickerConfig.Filter.ClassPaths.Add(UBrickLevelData::StaticClass()->GetClassPathName()); // Only show UBrickLevelData
	AssetPickerConfig.SelectionMode = ESelectionMode::Single; // Single asset selection
	// AssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateRaw(this, &SLevelEditorTool::OnAssetSelected);
	AssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateLambda([this, AssetPickerWindow](const FAssetData& SelectedAsset)
	{
		this->OnAssetSelected(SelectedAsset);
		FSlateApplication::Get().RequestDestroyWindow(AssetPickerWindow);
	});
	AssetPickerConfig.InitialAssetViewType = EAssetViewType::List;
	
	TSharedRef<SWidget> AssetPickerWidget = ContentBrowserModule.Get().CreateAssetPicker(AssetPickerConfig);

	// Create a modal window to host the asset picker
		

	AssetPickerWindow->SetContent(AssetPickerWidget);

	FSlateApplication::Get().AddWindow(AssetPickerWindow);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE
