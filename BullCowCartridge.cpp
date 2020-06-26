// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"


void UBullCowCartridge::BeginPlay()
{   
    // Call this function to begin game.
    Super::BeginPlay();
    // Create WordListPath from HiddenWord.txt.
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
  
    // Fill Isograms list utilizing lambda expression and predicate. 
    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, [](const FString& Word)
    {
        return Word.Len() > 4 && Word.Len() < 8 && IsIsogram(Word);
    });

    
    
    // Initialize game.
    InitGame();

    // Debug Line.
    PrintLine(TEXT("The number of valid words is %i"), Isograms.Num());
}

void UBullCowCartridge::InitGame()
{   
    // Print Introduction/Instructions
    PrintLine(TEXT("Welcome to Bull Cows!"));

    // Create random number in  range 0 - Isograms.NUM() - 1.
    Rand = FMath::RandRange(0, Isograms.Num() - 1);

    HiddenWord = Isograms[Rand]; // Set hidden word.

    Lives = HiddenWord.Len(); // Initialize lives.
    bGameOver = false; // Set game over false

    PrintLine(TEXT("The hidden word is: %s"), *HiddenWord); // Debug Line
    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("Type in your guess and \npress enter to continue...")); // Prompt player for guess.
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

void UBullCowCartridge::ProcessGuess(const FString& Guess)
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
        PrintLine(TEXT("Game Over! The Hidden Word was: ") + HiddenWord);
        EndGame();
        return;
    }

    // Show the player Bulls and Cows
    int32 Bulls, Cows;
    GetBullsCows(Guess, Bulls, Cows);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Bulls, Cows);

    PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word)
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

void UBullCowCartridge::GetBullsCows(const FString& Guess, int32& BullCount, int32& CowCount) const
{
    BullCount = 0;
    CowCount = 0;

    // If index Guess is same as index Hidden BullCount++ 
    // If not a bull was it a cow? CowCount ++
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            BullCount++;
            continue;
        }

        for (int32 CowIndex = 0; CowIndex < Guess.Len(); CowIndex++)
        {
                if (Guess[GuessIndex] == HiddenWord[CowIndex])
                {
                    CowCount++;
                }
        } 
    }
}