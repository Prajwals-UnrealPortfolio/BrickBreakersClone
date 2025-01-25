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

/**
 * Constructs the level editor tool's user interface and initializes its components.
 *
 * This method sets up the entire UI for the level editor, including:
 * - Initializing the grid of bricks
 * - Configuring color mappings for different brick types
 * - Creating UI controls for grid size adjustment (rows and columns)
 * - Adding buttons for saving, loading, and clearing the grid
 * - Creating buttons for selecting different brick types
 * - Setting up the grid visualization panel
 *
 * @note The method uses Slate UI framework for creating the interface
 * @note Initializes the grid with zero-filled data and sets the default brick type to NORMAL
 * @note Configures spin boxes for dynamic grid size modification
 * @note Adds color-coded buttons for different brick types
 *
 * @param InArgs Slate function arguments (unused in this implementation)
 */
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

/**
 * @brief Handles the selection of a specific brick type in the level editor.
 *
 * This method is triggered when a brick type button is clicked, updating the current
 * brick type for subsequent grid interactions. It sets the active brick type and
 * signals that the event has been processed.
 *
 * @param BrickType The type of brick selected by the user
 * @return FReply Indicates that the event has been handled by the UI system
 */
FReply SLevelEditorTool::OnBrickTypeClicked(EBRICK_TYPE BrickType)
{
	SetBrickType(BrickType);
	return FReply::Handled();
}

/**
 * Initiates the save level process by opening the save dialog.
 *
 * @brief Triggers the save dialog for the current level, allowing the user to specify a name and save location.
 * @return FReply::Handled() to indicate the event has been processed
 */
FReply SLevelEditorTool::OnSaveLevel()
{
	OnOpenSaveDialog();
	return FReply::Handled();
}

/**
 * Opens a modal dialog window for saving a level with a custom name.
 *
 * This method creates a modal dialog that allows the user to enter a name for the current level.
 * The dialog includes:
 * - A text prompt instructing the user to enter a level name
 * - An editable text box pre-populated with "NewLevel"
 * - A "Save" button that triggers the SaveLevelAs method with the entered name
 *
 * The dialog window is modal, preventing interaction with the main window until closed.
 * If a non-empty name is provided, the level is saved and the dialog is automatically closed.
 *
 * @note The dialog has a fixed size of 400x150 pixels and does not support maximizing or minimizing.
 */
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

/**
 * @brief Initiates the level loading process by opening an asset picker dialog.
 *
 * This method triggers the asset picker interface, allowing the user to select a previously saved level asset
 * to load into the level editor. After selection, the OnAssetSelected method will handle loading the chosen asset.
 *
 * @return FReply Indicates that the event has been handled by the method.
 *
 * @note This method is typically connected to a UI button or menu item for loading levels.
 * @see OnOpenAssetPicker()
 * @see OnAssetSelected()
 */
FReply SLevelEditorTool::OnLoadLevel()
{
	OnOpenAssetPicker();
	return FReply::Handled();
}

/**
 * Clears the entire grid and updates the visualization.
 *
 * This method performs two primary actions:
 * 1. Resets all grid cells to an empty state using ClearDataInGrids()
 * 2. Refreshes the grid's visual representation by calling UpdateGrid()
 *
 * @return FReply indicating that the event has been handled, preventing further propagation
 */
FReply SLevelEditorTool::OnClearGrid()
{
	ClearDataInGrids();
	UpdateGrid();
	UE_LOG(LogTemp, Log, TEXT("Grid cleared."));
	return FReply::Handled();
}

/**
 * @brief Rebuilds the grid visualization dynamically based on current grid state.
 *
 * This method clears the existing grid panel and reconstructs it by iterating through
 * each grid cell. For each cell, it creates a bordered widget with a color representing
 * the brick type and a text label showing the brick state.
 *
 * @details The method handles:
 * - Clearing existing grid children
 * - Iterating through grid rows and columns
 * - Creating interactive grid cells with mouse click handling
 * - Setting cell colors based on brick type
 * - Displaying brick type labels
 *
 * @note Interaction allows adding or removing bricks by clicking grid cells
 * @note Empty cells are displayed with blank text
 *
 * @see AddBrickToGrid()
 * @see RemoveBrickFromGrid()
 * @see GetColorByBrickType()
 */
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

/**
 * Saves a brick level data asset to the project's content directory.
 *
 * This method handles the process of creating or overwriting a level asset, including:
 * - Checking for existing assets
 * - Prompting user for overwrite confirmation
 * - Creating a new asset package
 * - Copying level data to the new asset
 * - Saving the package to disk
 *
 * @param LevelData The UBrickLevelData object containing the level configuration to save
 * @param AssetName The name of the asset to be created or overwritten
 *
 * @note If an asset with the same name already exists, a confirmation dialog is shown
 * @note Saves the asset in the "/Game/BrickBreakerLevels/" directory
 *
 * @see ShowNotification
 * @see UBrickLevelData
 */
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

/**
 * @brief Loads a brick level data asset from the specified asset path.
 *
 * @param AssetPath Full path to the brick level data asset to be loaded.
 *
 * @return bool True if the asset was successfully loaded and processed, false otherwise.
 *
 * @details This method attempts to load a UBrickLevelData asset and update the current level editor's grid configuration.
 * If successful, it:
 * - Sets the current level data
 * - Updates grid dimensions (rows and columns)
 * - Sets the cell size
 * - Resets the grid bricks array
 * - Populates the grid with brick information from the loaded asset
 *
 * @note The method uses StaticLoadObject to load the asset and performs type casting to ensure correct asset type.
 * @note Grid indices are calculated based on the cell size to map brick positions correctly.
 */
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

/**
 * @brief Handles the selection of a level asset from the asset picker.
 *
 * This method is called when a user selects an asset in the asset picker dialog. It validates the selected asset,
 * attempts to load the level data, and provides user feedback about the loading process.
 *
 * @param SelectedAsset The asset data of the selected level asset.
 *
 * @note If the asset is valid and successfully loaded, the grid is updated and a success notification is shown.
 * @note If the asset is invalid or loading fails, a warning is logged and a failure notification is displayed.
 */
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

/**
 * @brief Adds a brick to the grid at the specified row and column.
 *
 * @param Row The vertical position of the brick in the grid.
 * @param Column The horizontal position of the brick in the grid.
 * @param BrickState The type of brick to be added to the grid.
 *
 * @details This method adds a new brick to the grid if the specified location is valid and not already occupied.
 * It calculates the grid index, creates a new FBrickInfo with the correct position and brick type, 
 * and then updates the grid visualization.
 *
 * @note The method uses CellSize to calculate the pixel position of the brick.
 * @note The grid is updated only if the index is valid and the cell is not already filled.
 */
void SLevelEditorTool::AddBrickToGrid(int32 Row, int32 Column, EBRICK_TYPE BrickState)
{
	int32 Index = GetGridIndex(Row, Column);
	if (GridBricks.IsValidIndex(Index) && !GridBricks[Index]->BrickState != EMPTY)
	{
		GridBricks[Index] = FBrickInfo(Row * CellSize, Column * CellSize, BrickState);
		UpdateGrid();
	}
}

/**
 * @brief Removes a brick from the specified grid location.
 *
 * Removes the brick at the given row and column by setting its state to EMPTY.
 * If the grid index is valid and the current brick is not already empty, 
 * the brick state is updated and the grid visualization is refreshed.
 *
 * @param Row The row index of the brick to remove.
 * @param Column The column index of the brick to remove.
 *
 * @note This method triggers a grid update to reflect the change in the UI.
 */
void SLevelEditorTool::RemoveBrickFromGrid(int32 Row, int32 Column)
{
	int32 Index = GetGridIndex(Row, Column);
	if (GridBricks.IsValidIndex(Index) && GridBricks[Index]->BrickState != EMPTY)
	{
		GridBricks[Index]->BrickState = EMPTY;
		UpdateGrid();
	}
}

/**
 * @brief Saves the current level configuration as a new asset with the specified name.
 *
 * @details This method prepares the current level data for saving by:
 * - Validating the provided asset name
 * - Creating a new level data asset if none exists
 * - Collecting brick information from the current grid
 * - Setting grid size and cell size metadata
 * - Saving the asset using the SaveDataAsset method
 *
 * @param LevelName The desired name for the level asset
 *
 * @note If the asset name is invalid or no bricks are present, the save operation will be skipped
 * @note Logs a success message upon successful asset creation
 *
 * @see IsAssetNameValid()
 * @see SaveDataAsset()
 */
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

/**
 * Validates the provided asset name for a new level asset.
 *
 * @param AssetName The proposed name for the level asset.
 * @return bool True if the asset name is valid and unique, false otherwise.
 *
 * @details Checks two primary conditions for asset name validity:
 * 1. The asset name cannot be empty.
 * 2. An asset with the same name must not already exist in the BrickBreakerLevels directory.
 *
 * @note Logs a warning message if the asset name is invalid, providing context for the failure.
 * @note Assumes the standard asset path for BrickBreaker levels is "/Game/BrickBreakerLevels/".
 */
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

/**
 * Displays a notification message to the user with a specified completion state.
 *
 * @param Message The text message to be displayed in the notification.
 * @param State The completion state of the notification (e.g., success, failure, pending).
 *
 * @note The notification will automatically fade out after 3 seconds.
 * @note Uses Slate's notification system to provide visual feedback to the user.
 */
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

/**
 * @brief Adjusts the grid size while preserving existing brick data.
 *
 * This method resizes the grid by creating a new grid array and transferring existing brick information.
 * If the grid size increases, new cells are initialized with empty bricks. If the grid size decreases,
 * only the bricks within the new grid dimensions are preserved.
 *
 * @note The method maintains the relative positions of existing bricks in the new grid configuration.
 * @note After resizing, the grid visualization is updated to reflect the new grid state.
 *
 * @see UpdateGrid()
 */
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

/**
 * @brief Clears all bricks in the grid, resetting them to an empty state.
 *
 * Iterates through each cell in the grid and sets the brick to an empty state.
 * This method resets the entire grid to its initial configuration with no bricks.
 * Each brick's position is calculated based on its row and column, maintaining
 * the original grid layout but with no active brick types.
 *
 * @note The method uses nested loops to traverse all rows and columns of the grid.
 * @note Brick positions are calculated using row and column indices multiplied by cell size.
 * @note After execution, all grid cells will contain an EMPTY brick type.
 */
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

/**
 * @brief Retrieves the color associated with a specific brick type.
 *
 * Searches through the BricksColors array to find the matching brick type and returns its corresponding color.
 * If no matching brick type is found, returns the color of the first brick type in the array.
 *
 * @param BrickState The type of brick for which to retrieve the color.
 * @return FLinearColor The color associated with the specified brick type.
 */
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

/**
 * @brief Sets the current brick type for the level editor.
 *
 * @param BrickType The type of brick to be used when adding new bricks to the grid.
 *
 * @note This method updates the internal brick type state, which determines the type of brick
 * that will be placed when interacting with the grid.
 */
void SLevelEditorTool::SetBrickType(EBRICK_TYPE BrickType)
{
	CurrentBrickType = BrickType;
}

/**
 * Opens an asset picker dialog to select a Brick Level Data asset.
 *
 * This method creates a modal window with a content browser asset picker, configured to:
 * - Display only UBrickLevelData assets
 * - Allow single asset selection
 * - Provide a list view of available assets
 *
 * When an asset is selected, it triggers the OnAssetSelected method and closes the picker window.
 *
 * @note Uses the Content Browser module to create the asset picker
 * @note Creates a modal window with a fixed size of 800x600 pixels
 * @note Prevents window maximization and minimization
 */
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
