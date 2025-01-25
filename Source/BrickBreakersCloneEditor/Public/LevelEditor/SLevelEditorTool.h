#pragma once

#include "CoreMinimal.h"
#include "LevelData/BrickLevelData.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Notifications/SNotificationList.h"

class UBrickLevelData;

class SLevelEditorTool : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SLevelEditorTool) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	UPROPERTY()
	UBrickLevelData* CurrentLevelData = nullptr;

	TArray<TOptional<FBrickInfo>> GridBricks; // Represents the grid layout
	int32 GridRows = 10;
	int32 GridCols = 10;
	float CellSize = 100.f;
	TSharedPtr<SSpinBox<int32>> RowSpinBox;
	TSharedPtr<SSpinBox<int32>> ColSpinBox;
	TSharedRef<SGridPanel> GridPanel = SNew(SGridPanel);
	TArray<FBrickColor> BricksColors;
	TEnumAsByte<EBRICK_TYPE> CurrentBrickType;
	inline static const UEnum* EBrickEnumType = FindObject<UEnum>(ANY_PACKAGE, TEXT("EBRICK_TYPE"));

private:
	
	FReply OnSaveLevel();
	FReply OnLoadLevel();
	FReply OnClearGrid();
	FReply OnBrickTypeClicked(EBRICK_TYPE BrickType);
	void OnOpenAssetPicker();
	void OnOpenSaveDialog();

	void UpdateGrid();
	void SaveDataAsset(UBrickLevelData* LevelData, const FString& AssetName);
	bool LoadDataAsset(const FString& AssetPath);
	void AddBrickToGrid(int32 Row, int32 Column, EBRICK_TYPE BrickState);
	void RemoveBrickFromGrid(int32 Row, int32 Column);
	void OnAssetSelected(const FAssetData& SelectedAsset);
	void SaveLevelAs(const FString& LevelName);
	bool IsAssetNameValid(const FString& AssetName) const;
	void ShowNotification(const FString& Message, SNotificationItem::ECompletionState State);
	void OnGridSizeChanged();
	void ClearDataInGrids();
	FLinearColor GetColorByBrickType(EBRICK_TYPE BrickState);
	void SetBrickType(EBRICK_TYPE BrickType);
	
	FORCEINLINE int32 GetGridIndex(int32 Row, int32 Col) const { return Row * GridCols + Col; }
};
