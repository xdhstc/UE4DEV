
#include "RigControllerWindow.h"

#include "TestPlugin.h"
//#include "Editor/UnrealEd/Public/EditorDirectories.h"
//#include "Editor/UnrealEd/Private/FileHelpers.cpp"
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"

#define LOCTEXT_NAMESPACE "RigControllerWindow"

//const FName testTabeName(TEXT("ttt"));
const FName fileBrowseTabName(TEXT("FileBrowse"));
RigControllerWindow::RigControllerWindow()
{
	
}




void RigControllerWindow::Construct(const FArguments & args)
{

	//FGlobalTabmanager::Get()->RegisterTabSpawner(fileBrowseTabName, FOnSpawnTab::CreateRaw(this, &RigControllerWindow::createFileTab)).SetDisplayName(FText::FromString(TEXT("aaaa")));
	//TSharedRef<SDockTab> Tab = SNew(SDockTab);



	//Tab = FGlobalTabmanager::Get()->InvokeTab(fileBrowseTabName);

	//TSharedRef<FTabManager> tabManager = FGlobalTabmanager::Get()->NewTabManager(Tab);
	//tabManager->RegisterTabSpawner(fileBrowseTabName, FOnSpawnTab::CreateRaw(this, &RigControllerWindow::createFileTab)).SetDisplayName(FText::FromString(TEXT("aaaa")));


	ChildSlot
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(3)
			.AutoWidth()
			
			[
				SNew(SBorder)
				//.Content()

				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(3)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("label1", "curve file  "))
					]
				+ SHorizontalBox::Slot()//.FillWidth(5.f)
					[
						curvePathEditText
					]
				+ SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SNew(SButton)
						.Text(LOCTEXT("browseBtn", "Browse"))
					.OnClicked(this, &RigControllerWindow::browseBtnClicked)
					]


					]

				+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(3)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("label2", "character "))

					]
				+ SHorizontalBox::Slot()
					[
						comboBtn
					]


					]
				+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(3)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
					[
						SNew(SButton)
						.Text(LOCTEXT("importBtn", "import"))
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.OnClicked(this, &RigControllerWindow::importBtnClicked)
					]


					]
				+ SVerticalBox::Slot()
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						[
							SNew(SImage)

							.ColorAndOpacity(FSlateColor::FSlateColor(FLinearColor::Black))
						]
					]


				]












			]
			+ SHorizontalBox::Slot()
				.Padding(3)
			[
				SNew(SBorder)
				[
					curvePanel
				]
			]


		];




	/*
	ChildSlot
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.FillWidth(1)
			[
				SNew(SSplitter)
				.ResizeMode(ESplitterResizeMode::Fill)
				+ SSplitter::Slot()
				[
					SNew(SSplitter)
					.Orientation(Orient_Horizontal)
					.ResizeMode(ESplitterResizeMode::Fill)
					+ SSplitter::Slot()
					[
						SNew(SBorder)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Col0Row0", "Col 0 Row 0"))
						]
					]
					+ SSplitter::Slot()
					[
						SNew(SBorder)
						[
							SNew(SCurvePanel)
						]
					]
				]
			]
		];


		*/








}

TSharedRef<SDockTab> RigControllerWindow::createFileTab(const FSpawnTabArgs & args)
{
	return 

		SNew(SDockTab)
		.Label(LOCTEXT("aaa", "bbb"))
		.TabRole(ETabRole::MajorTab)
		.ContentPadding(5)
		[
			SNew(SBorder)
			.Padding(4)

		
		];
}

FReply RigControllerWindow::browseBtnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("browse btn clicked"));
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	bool bOpened = false;
	TArray<FString> OutOpenFilenames;

	if (DesktopPlatform)
	{
		bOpened = DesktopPlatform->OpenFileDialog(
			FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),
			NSLOCTEXT("UnrealEd", "ImportFile", "Import File").ToString(),
			FString("d:/"),
			TEXT(""),
			TEXT("JSON (*.json)|*.json"),
			EFileDialogFlags::None,
			OutOpenFilenames
		);
	}

	bOpened = (OutOpenFilenames.Num() > 0);




	/*
	TArray<FString> OpenedFiles;
	FString DefaultLocation(FEditorDirectories::Get().GetLastDirectory(ELastDirectory::UNR));

	if (FileDialogHelpers::OpenFiles(NSLOCTEXT("UnrealEd", "ImportScene", "Import Scene").ToString(), FEditorFileUtils::GetFilterString(FI_ImportScene), DefaultLocation, EFileDialogFlags::None, OpenedFiles))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"),*OpenedFiles[0]);

	}
	*/

	if (bOpened)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *OutOpenFilenames[0]);
		curvePathEditText->SetText(FText::FromString(OutOpenFilenames[0]));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("no file sel"));
	}







	return FReply::Handled();

}

FReply RigControllerWindow::importBtnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("import btn clicked"));


	FString FileContents;
	FString FileName = curvePathEditText->GetText().ToString();
	FFileHelper::LoadFileToString(FileContents, *FileName);

	//TSharedPtr< FJsonObject > ObjectPtr;
	//FString JsonValue = "[{\"author\":\"4446545646544\"},{\"name\":\"jack\"}]";
	//TArray<TSharedPtr<FJsonValue>> JsonParsed;
	//TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(FileContents);

	//bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonParsed);

	TSharedPtr< FJsonObject > ObjectPtr;
	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(FileContents);
	
	bool BFlag = FJsonSerializer::Deserialize(Reader, ObjectPtr);


	if (BFlag)
	{
		UE_LOG(LogTemp, Warning, TEXT("ssss"));
		
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::SanitizeFloat( ObjectPtr->GetNumberField(FString("age"))));
		TMap<FString, TSharedPtr<FJsonValue>> jsonMap = ObjectPtr->Values;

		curvePanel->loadJson(jsonMap);



	
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::FromInt(ObjectPtr->);
		//ObjectPtr->GetStringField(FString("age"));
	}


	

	
	return FReply::Handled();
}


