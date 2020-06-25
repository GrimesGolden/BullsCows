// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay()
{   
    // Call this function to begin game.
    Super::BeginPlay();
    // Create WordListPath from HiddenWord.txt.
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    // Create WordList using WordListPath
    FFileHelper::LoadFileToStringArray(WordList, *WordListPath);
    // Create ValidWords list using GetValidWords().
    TArray<FString> ValidWords = GetValidWords(WordList);
    
    // Initialize game.
    InitGame();

    // Debug Line.
    PrintLine(TEXT("The number of possible words is %i"), WordList.Num());
    PrintLine(TEXT("The number of valid words is %i"), ValidWords.Num());
    PrintLine(TEXT("The HiddenWord is: %s"), *HiddenWord);
}

void UBullCowCartridge::OnInput(const FString& Input) 
// On player input
{
    if (bGameOver)
    // If game over then clearscreen and initialize game.
    {
        ClearScreen();
        InitGame();
    }
    else // Checking PlayerGuess
    {   
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::InitGame()
{   
    // Print Introduction/Instructions
    PrintLine(TEXT("Welcome to Bull Cows!"));

    HiddenWord = TEXT("cake"); // Set hiddenword.

    Lives = HiddenWord.Len(); // Initialize lives.
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("Type in your guess and \npress enter to continue...")); // Prompt player for guess.
}

void UBullCowCartridge::ProcessGuess(FString Guess)
// Analyse guess input, if valid, parse for win or loss.
{   
    if (Guess == HiddenWord)
        {
            PrintLine(TEXT("You have Won!"));
            EndGame();
            return;
        }

    if (Guess.Len() != HiddenWord.Len())
        {
            PrintLine(TEXT("ERROR: Word length is %i characters."), HiddenWord.Len());
            return;
        }
    
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("Isogram = No repeating letters: guess again"));
        return;
    }

    // If Player didn't win and didn't make an erroneus guess then...
    --Lives;
    
    if (Lives > 0)
    {   
        PrintLine(TEXT("Incorrect Guess: You have %i lives left."), Lives);
    }

    else if (Lives <= 0) 
    {   
        ClearScreen();
        PrintLine(TEXT("Game Over: The Hidden Word was ") + HiddenWord);
        EndGame();
        return;
    }
}

TArray<FString> UBullCowCartridge::GetValidWords(TArray<FString> Words) const
// From TArray of strings, create new list of 4-8 letter isograms.
{   
    TArray<FString> ValidWords;
    for (int32 Index = 0; Index != Words.Num(); Index++)
    {
        if (Words[Index].Len() >= 4 && Words[Index].Len() <= 8 && IsIsogram(Words[Index]))
        {      
            ValidWords.Emplace(Words[Index]);
        }
    }

    return ValidWords;
}

bool UBullCowCartridge::IsIsogram(FString Word) const
// Determine if input string has repeating letters (!isogram).
{  
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {   
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    
    return true;
}

void UBullCowCartridge::EndGame()
// Set game over to true, which breaks game loop and declares message.
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again..."));
}