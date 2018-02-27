
#include "SCurvePanel.h"

#include "TestPlugin.h"
//#include "Runtime/Engine/Classes/Engine/World.h"
#define LOCTEXT_NAMESPACE "RigControllerWindow"

SCurvePanel::SCurvePanel()
{
	textSelected = false;
	winState = WindowState::Normal;
	canvasScale = 1.0f;
	panelMaxWidth = 1500.f;
	panelMaxHeight = 980.f;
}

void SCurvePanel::Construct(const FArguments & args)
{
	ChildSlot
		[
			ctlCanvas
		];
	
	//Advanced_SetDesiredSize(FVector2D(20, 20));
	//FSlateRenderTransform::FTransform2D(FVector2D(100.f, 100.f));
	//ctlCanvas->SetRenderTransform(FSlateRenderTransform::FTransform2D(FVector2D(0, 0)));
	SetRenderTransform(FSlateRenderTransform::FTransform2D(FVector2D(0, 0)));
	
	//ctlCanvas->SetRenderTransform
	//int aa = ctlCanvas->AddSlot();
	testCurveSlot1 = &ctlCanvas->AddSlot();
	testCurveSlot1->Position(FVector2D(350, 170));
	testCurveSlot1->Size(FVector2D(200.0f, 80.0f));
	testCurve1->setCanvasSlot(testCurveSlot1);
	//testCurve1->setPanretPanel(this);
	//testCurve1->SetToolTipText(LOCTEXT("ccc", "curve1"));
	
	TArray<FVector2D> vecList1;
	vecList1.Add(FVector2D(0, 0));
	vecList1.Add(FVector2D(30, 10));
	vecList1.Add(FVector2D(50, 10));
	vecList1.Add(FVector2D(0, 0));
	testCurve1->setPointArray(vecList1,false,FString("curve1"));

	testCurveSlot1->AttachWidget(testCurve1.Get().AsShared());
	curveList.Add(testCurve1);




	testCurveSlot = &ctlCanvas->AddSlot();
	testCurveSlot->Position(FVector2D(300, -40));
	testCurveSlot->Size(FVector2D(100.0f, 40.0f));
	testCurve->setCanvasSlot(testCurveSlot);
	//testCurve->SetToolTipText(LOCTEXT("bbbb", "curve"));

	TArray<FVector2D> vecList;
	vecList.Add(FVector2D(10, 10));
	vecList.Add(FVector2D(50, 40));
	vecList.Add(FVector2D(100, 10));
	vecList.Add(FVector2D(10, 10));


	testCurve->setPointArray(vecList, false, FString("curve"));
	testCurveSlot->AttachWidget(testCurve.Get().AsShared());

	curveList.Add(testCurve);

	



	testBtn->SetToolTipText(LOCTEXT("aaa", "4444444"));
	
	testBtn->SetOnClicked(FOnClicked::CreateRaw(this, &SCurvePanel::testBtnClicked));
	testBtnSlot = &ctlCanvas->AddSlot();
	testBtnSlot->HAlign(HAlign_Center);
	testBtnSlot->VAlign(VAlign_Center);
	testBtnSlot->Position(FVector2D(200, 40));
	testBtnSlot->Size(FVector2D(100.0f, 40.0f));
	testBtnSlot->AttachWidget(testBtn.Get().AsShared());

}


void SCurvePanel::initPanel(TMap<FString, TSharedPtr<FJsonValue>> _jsonMap)
{
	clearPanel();
	curveDataArray.Empty();
	//float minX = 0.f;
	//float minY = 0.f;
	//float maxX = 0.f;
	//float maxY = 0.f;
	TArray<float> xAllArray;
	TArray<float> yAllArray;
	for (TMap<FString, TSharedPtr<FJsonValue>>::TIterator It(_jsonMap); It; ++It)
	{
		CurveData curveData;
		TSharedPtr< FJsonObject > contentPtr = It.Value()->AsObject();
		curveData.name = It.Key();
		curveData.isStatic = contentPtr->GetBoolField("static");
		TSharedPtr< FJsonObject > vertexDictPtr = contentPtr->GetObjectField(FString("vertex_dict"));
		TMap<FString, TSharedPtr<FJsonValue>> vertexDictMap = vertexDictPtr->Values;
		vertexDictMap.KeySort([](FString A, FString B) {return A < B; });
		TArray<float> curveXArray;
		TArray<float> curveYArray;
		for (TMap<FString, TSharedPtr<FJsonValue>>::TIterator Ik(vertexDictMap); Ik; ++Ik)
		{
			//FString vName = Ik.Key();

			const  TArray<TSharedPtr<FJsonValue>> jsonVexList = Ik.Value()->AsArray();
			float tmpX = jsonVexList[0]->AsNumber();
			float tmpY = -jsonVexList[1]->AsNumber();
			xAllArray.Add(tmpX);
			yAllArray.Add(tmpY);
			curveXArray.Add(tmpX);
			curveYArray.Add(tmpY);
			curveData.pointArray.Add(FVector2D(tmpX, tmpY));
		}
		curveXArray.Sort([](float A, float B) {return A < B; });
		curveYArray.Sort([](float A, float B) {return A < B; });
		curveData.offsetVector = FVector2D(curveXArray[0], curveYArray[0]);
		//curveData.xMin = curveXArray[0];
		//curveData.yMin = curveYArray.Last();
		//curveData.width = curveXArray.Last() - curveXArray[0];
		//curveData.height = curveYArray.Last() - curveYArray[0];
		curveData.size = FVector2D(curveXArray.Last() - curveXArray[0], curveYArray.Last() - curveYArray[0]);
		curveDataArray.Add(curveData);
	}

	xAllArray.Sort([](float A, float B) {return A < B; });
	yAllArray.Sort([](float A, float B) {return A < B; });

	float xMin = xAllArray[0];
	float xMax = xAllArray.Last();
	float yMin = yAllArray[0];
	float yMax = yAllArray.Last();

	//UE_LOG(LogTemp, Warning, TEXT("minX:%s"), *FString::SanitizeFloat(xMin));
	//UE_LOG(LogTemp, Warning, TEXT("maxX:%s"), *FString::SanitizeFloat(xMax));
	//UE_LOG(LogTemp, Warning, TEXT("minY:%s"), *FString::SanitizeFloat(yMin));
	//UE_LOG(LogTemp, Warning, TEXT("maxY:%s"), *FString::SanitizeFloat(yMax));

	float xScale = panelMaxWidth / (xMax - xMin);
	float yScale = panelMaxHeight / (yMax - yMin);
	float tmpCanvasScale = xScale < yScale ? xScale : yScale;

	FVector2D AllOffsetVector = FVector2D(-xMin, -yMin);

	for (int i = 0; i < curveDataArray.Num(); i++)
	{
		TArray<FVector2D> vexList;
		for (int j = 0; j < curveDataArray[i].pointArray.Num(); j++)
		{
			vexList.Add((curveDataArray[i].pointArray[j] - curveDataArray[i].offsetVector)*tmpCanvasScale);
			//myDebug(((curveDataArray[i].pointArray[j] - curveDataArray[i].offsetVector)*canvasScale).ToString());
		}


		SCanvas::FSlot *myCurveSlot = &ctlCanvas->AddSlot();
		myCurveSlot->Position((curveDataArray[i].offsetVector + AllOffsetVector)*tmpCanvasScale);
		if (!curveDataArray[i].isStatic)
		{
			myCurveSlot->Size(curveDataArray[i].size*tmpCanvasScale);
		}
		
		TSharedRef<SCurveSlate> myCurve = SNew(SCurveSlate);
		myCurve->setCanvasSlot(myCurveSlot);
		myCurve->setPointArray(vexList, curveDataArray[i].isStatic, curveDataArray[i].name);
		myCurveSlot->AttachWidget(myCurve.Get().AsShared());
		curveList.Add(myCurve);
	}











	/*

	for (TMap<FString, TSharedPtr<FJsonValue>>::TIterator It(_jsonMap); It; ++It)
	{
		TSharedRef<SCurveSlate> myCurve = SNew(SCurveSlate);
		SCanvas::FSlot *myCurveSlot = &ctlCanvas->AddSlot();
		myCurve->setCanvasSlot(myCurveSlot);
		TArray<FVector2D> vexList;

		FString curveName = It.Key();
		TSharedPtr< FJsonObject > contentPtr = It.Value()->AsObject();
		
		bool isStatic = contentPtr->GetBoolField("static");
		TSharedPtr< FJsonObject > vertexDictPtr = contentPtr->GetObjectField(FString("vertex_dict"));
		TMap<FString, TSharedPtr<FJsonValue>> vertexDictMap = vertexDictPtr->Values;
		vertexDictMap.KeySort([](FString A, FString B) {return A < B; });


		TArray<float> xArray;
		TArray<float> yArray;
		for (TMap<FString, TSharedPtr<FJsonValue>>::TIterator Ik(vertexDictMap); Ik; ++Ik)
		{
			const  TArray<TSharedPtr<FJsonValue>> jsonVexList = Ik.Value()->AsArray();
			float xVal = (jsonVexList[0]->AsNumber() + xOffset)*tmpCanvasScale;
			float yVal = (-jsonVexList[1]->AsNumber() + yOffset)*tmpCanvasScale;
			xArray.Add(xVal);
			yArray.Add(yVal);
			vexList.Add(FVector2D(xVal, yVal));
		}
		xArray.Sort([](float A, float B) {return A < B; });
		yArray.Sort([](float A, float B) {return A < B; });

		float xMin = xArray[0];
		float xMax = xArray.Last();
		float yMin = yArray[0];
		float yMax = yArray.Last();


		myCurveSlot->Position(FVector2D(xMin, yMin));
		myCurveSlot->Size(FVector2D((xMax-xMin), (yMax- yMin)));


		myCurve->setPointArray(vexList, isStatic, curveName);
		myCurveSlot->AttachWidget(myCurve.Get().AsShared());

		curveList.Add(myCurve);


	}

	*/


}

void SCurvePanel::clearPanel()
{
	for (int i = 0; i < curveList.Num(); i++)
	{
		ctlCanvas->RemoveSlot(curveList[i]);
	}
	curveList.Empty();

}






FReply SCurvePanel::OnMouseWheel(const FGeometry & MyGeometry, const FPointerEvent & mouseEvent)
{
	
	const FVector2D WidgetSpaceCursorPos = MyGeometry.AbsoluteToLocal(mouseEvent.GetScreenSpacePosition());
	const int32 ZoomLevelDelta = FMath::FloorToInt(mouseEvent.GetWheelDelta());
	float aa = mouseEvent.GetWheelDelta()/10.f;

	canvasScale = canvasScale *(1 + aa  );

	canvasScale = canvasScale > 0.1f ? canvasScale : 0.1f;
	canvasScale = canvasScale < 5.0f ? canvasScale : 5.0f;
	FSlateRenderTransform trans(FScale2D(canvasScale, canvasScale), FVector2D(0, 0));
	//SetRenderTransform(trans);

	for (int i = 0; i < curveList.Num(); i++)
	{
		curveList[i]->SetRenderTransform(trans);
		curveList[i]->canvasSlot->Position(curveList[i]->canvasSlot->PositionAttr.Get()*(1 + aa));
	}



	//testCurveSlot->GetWidget()->setren
	//testBtn->SetContentScale(FVector2D(canvasScale, canvasScale));
	//testBtn->SetDesiredSizeScale(FVector2D(canvasScale, canvasScale));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::SanitizeFloat(canvasScale));
	
	return FReply::Handled();
}




FReply SCurvePanel::OnMouseButtonDown(const FGeometry & MyGeometry, const FPointerEvent & mouseEvent)
{

	if (mouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		UE_LOG(LogTemp, Warning, TEXT("canvas left click"));
		{
			for (int i = 0; i < curveList.Num(); i++)
			{
				if (curveList[i]->isHovered)
				{
					UE_LOG(LogTemp, Warning, TEXT("hover"));
					winState = WindowState::CurveMove;
					selectCurve = &curveList[i];
					selectCurve->Get().isSelected = true;
					clickPos = mouseEvent.GetScreenSpacePosition();
					selectCurvePos = selectCurve->Get().canvasSlot->PositionAttr.Get();
					return FReply::Handled();;
				}


			}
			UE_LOG(LogTemp, Warning, TEXT(" no hover"));
		}


	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("canvas right click"));
		winState = WindowState::CanvasMove;
		clickPos = mouseEvent.GetScreenSpacePosition();
		FSlateApplication::Get().GetPlatformCursor().Get()->SetType(EMouseCursor::GrabHand);
		//canvasPos = ctlCanvas->GetRenderTransform()->GetTranslation();

		canvasPos = GetRenderTransform()->GetTranslation();

	}

	

	UE_LOG(LogTemp, Warning, TEXT("%s"),*mouseEvent.GetScreenSpacePosition().ToString());
	return FReply::Handled();
}

FReply SCurvePanel::OnMouseButtonUp(const FGeometry & MyGeometry, const FPointerEvent & mouseEvent)
{
	switch (winState)
	{
	case Normal:
		//FSlateApplication::Get().GetPlatformCursor().Get()->SetType(EMouseCursor::Default);
		break;

	case CurveMove:
		//selectCurve->Get().canvasSlot->Position(selectCurvePos + mouseEvent.GetScreenSpacePosition() - clickPos);
		UE_LOG(LogTemp, Warning, TEXT("%s"),*selectCurve->Get().canvasSlot->PositionAttr.Get().ToString());
		selectCurve->Get().isSelected = false;
		winState = WindowState::Normal;
		break;

	case CanvasMove:
		winState = WindowState::Normal;
		FSlateApplication::Get().GetPlatformCursor().Get()->SetType(EMouseCursor::Default);
		break;

	default:
		break;
	}




	return FReply::Handled();
}

FReply SCurvePanel::OnMouseMove(const FGeometry & MyGeometry, const FPointerEvent & mouseEvent)
{
	switch (winState)
	{
	case Normal:
		FSlateApplication::Get().GetPlatformCursor().Get()->SetType(EMouseCursor::Default);
		break;



		break;
	case CurveMove:
		selectCurve->Get().canvasSlot->Position(selectCurvePos + mouseEvent.GetScreenSpacePosition() - clickPos);


		break;

	case CanvasMove:
		//ctlCanvas->SetRenderTransform(FSlateRenderTransform::FTransform2D(canvasPos + mouseEvent.GetScreenSpacePosition()-clickPos));
		SetRenderTransform(FSlateRenderTransform::FTransform2D(canvasPos + mouseEvent.GetScreenSpacePosition() - clickPos));
		FSlateApplication::Get().GetPlatformCursor().Get()->SetType(EMouseCursor::GrabHandClosed);
		
		break;
	default:
		break;
	}




	return FReply::Handled();
}

FReply SCurvePanel::testBtnClicked()
{
	
	UE_LOG(LogTemp, Warning, TEXT("btn clicked1"));
	//ctlCanvas->RemoveSlot(testCurve);
	ctlCanvas->SetRenderTransform(FSlateRenderTransform::FTransform2D(FVector2D(200.f, 200.f)));
	

	return FReply::Handled();
}






void SCurvePanel::textLeave(const FPointerEvent &mouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("text leave"));

}
/*
void SCurvePanel::doCanvasMove(FVector2D _pos)
{
	for (int i = 0; i < curveList.Num(); i++)
	{
		curveList[i]->canvasSlot->Position(curveList[i]->screenLocation + _pos - clickPos);
	}



}

void SCurvePanel::doCanvasMoveEnd(FVector2D _pos)
{
	for (int i = 0; i < curveList.Num(); i++)
	{
		curveList[i]->screenLocation += _pos - clickPos;
	}

}
*/
void SCurvePanel::receiveMsg()
{
	UE_LOG(LogTemp, Warning, TEXT("rec msg"));
}


void SCurvePanel::myDebug(FString _inStr)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"),*_inStr);
}


