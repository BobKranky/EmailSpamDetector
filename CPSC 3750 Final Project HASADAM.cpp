// Hasan Raza and Adam Moore
// CPSC 3750
// Project AI Spam Filter Using Supervised Learning

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

int spamEmails = 0;      // Counts spam emails
int notSpamEmails = 0;   // Counts non spam emails
int spamEmails2 = 0;     // Counts spam emails during testing
int notSpamEmails2 = 0;  // Counts non spam emails during testing
int rightGuess = 0;      // Number of correct predictions during testing
int wrongGuess = 0;      // Number of wrong predictions during testing
int spamGuess = 0;       // Number of correct spam predictions during testing
int nonSpamGuess = 0;    // Number of correct non spam predictions during testing
bool emailState = false; // Email state of chosen email, preset to false but is overwritten when an email is chosen for testing of supervised learning
double avgRatio = 0;     // Average score
double value = 2;        // If result above this value it is predicted to be spam

std::vector<std::string> spamDatabase;  // Spam word database
std::vector<int> spamDatabaseWordcount; // Counts occurences of spam words

std::vector<std::string> nonSpamDatabase;
std::vector<int> nonSpamDatabaseWordcount;

std::vector<std::string> emailWords;
std::string states[2] = {"not spam", "spam"};

// This project works by first splitting up the contents of the email into different parts.
// Once they have been added, it checks the contents against the spam database.
// If the email seems to be more similar to one database or the other, it gives its evaluation
// Once the AI has given its evaluation, it recieves the true nature of the email, whether spam or not.
// The AI then adds the email to the correct database and waits for the next email.

std::string removePunctuation(const std::string &input) // removes all punctuation and leaves only spaces
{
    std::string result;
    for (char c : input)
    {
        if (!std::ispunct(static_cast<unsigned char>(c)))
        {
            result += std::tolower(c);
        }
    }
    return result;
}

void SavetoVector(std::string test)     // this function rids the chosen line of all punctuation marks, then saves the words to a vector email words
{
    std::string noPunc = removePunctuation(test);

    std::istringstream iss(noPunc);
    std::string word;
    while (iss >> word)
    {
        emailWords.push_back(word);
    }
}

int SpamWordChecker(std::vector<std::string> words)     // Checks each word of the email against the spamdatabase, it then saves the number of times those words were used in spam emails by adding them to wordcounter.
{
    int numSpamWords = 0;
    int spamWordCounter = 0;
    for (int i = 0; i < spamDatabase.size(); i++)       // Iterates through database
    {
        for (int j = 0; j < words.size(); j++)          // Iterates through email words
        {
            if (words.at(j) == spamDatabase.at(i))      // If a word == a word in the database
            {
                spamWordCounter += spamDatabaseWordcount.at(i); // Updates word counter
                numSpamWords++;
            }
        }
    }
    // std::cout << "Words associated with spam were found " << numspamwords << " times." << std::endl;
    // std::cout << "Number of times the words occured in spam database was " << spamWordCounter << " times." << std::endl;
    return spamWordCounter;
}

int NonSpamWordChecker(std::vector<std::string> words)  // Checks each word of the email against the nonspamdatabase, it then saves the number of times those words were used in nonspam emails by adding them to wordcounter.
{
    int numNonSpamWords = 0;
    int wordCounter = 0;

    for (int i = 0; i < nonSpamDatabase.size(); i++) // Iterates through database
    {
        for (int j = 0; j < words.size(); j++) // Iterates through email words
        {
            if (words.at(j) == nonSpamDatabase.at(i)) // If a word == a word in the database
            {
                wordCounter += nonSpamDatabaseWordcount.at(i); // Updates word counter
                numNonSpamWords++;
            }
        }
    }
    // std::cout << "Words associated with spam were found " << numNonSpamWords << " times." << std::endl;
    // std::cout << "Number of times the words occured in spam database was " << nonSpamWordCounter << " times." << std::endl;

    return wordCounter;
}

double Evaluation()     // Predicts whether email is spam or not by counting the occurences of words that are in the email and are present in both databases. If the email has more words associated with spam then the email thinks its spam and vice versus.
{
    double spamRatio = 0.0; // Resets ratios for every new email
    double nonspamRatio = 0.0;
    double finalRatio = 0.0;

    spamRatio = SpamWordChecker(emailWords);       // Checks how many words in the email are present in the spam database, and how common those words are
    nonspamRatio = NonSpamWordChecker(emailWords); // Checks how many words in the email are present in the not spam database, and how common those words are

    if (spamEmails == 0 || notSpamEmails == 0 || nonspamRatio == 0) // Error catcher to make sure that we are never dividing by 0 in any of our calculations.
    {
        return 0.0;
    }
    else
    {
        if (finalRatio == finalRatio && finalRatio < 100) // Error catcher to make sure that final ratio is never nan or inf
        {
            finalRatio = (spamRatio / spamEmails) / (nonspamRatio / notSpamEmails);
            avgRatio = avgRatio + finalRatio;
            // std::cout << "Evaluation ratio: " << finalRatio <<std::endl; // Not needed but helpful to know if any emails are evaluating incorrectly.
        }
        else
        {
        }
    }

    // The final ratio of greater than the variable "value" indicates that, on average, the words associated with spam occur more frequently per email than those associated with non-spam in the email database.
    // This ratio accounts for the differences in the number of spam and non-spam emails in the database, however, it does not account for words shared equally or are indeterminate of spam and not spam.
    return finalRatio;
}

void UpdateDatabase()   // Updates database with new email and clears emailswords vector in preparation for next line
{
    if (emailState == true) // Saves to spam email database
    {
        for (int j = 0; j < emailWords.size(); j++) // Iterates through email words
        {
            bool found = false;
            for (int i = 0; i < spamDatabase.size(); i++) // Iterates through database
            {
                if (emailWords.at(j) == spamDatabase.at(i)) // If word already present in database, adds 1 to word count
                {
                    spamDatabaseWordcount.at(i)++;
                    found = true; // Sets found to true if the word is present in database
                    break;
                }
            }
            if (!found) // If the word is not already present loop adds new word to the end of the vector
            {
                spamDatabase.push_back(emailWords.at(j));
                spamDatabaseWordcount.push_back(1);
            }
        }
    }

    else // Saves to spam not email database
    {
        for (int j = 0; j < emailWords.size(); j++)
        {
            bool found = false;
            for (int i = 0; i < nonSpamDatabase.size(); i++)
            {
                if (emailWords.at(j) == nonSpamDatabase.at(i)) // If word already present in database, adds 1 to word count
                {
                    nonSpamDatabaseWordcount.at(i)++;
                    found = true;
                    break;
                }
            }
            if (!found) // If the word is not already present loop adds new word to the end of the vector
            {
                nonSpamDatabase.push_back(emailWords.at(j));
                nonSpamDatabaseWordcount.push_back(1);
            }
        }
    }

    emailWords.clear(); // Clears vector for next email
}

void Testing(const std::string &filename)   // Evaluates the testing emails, and then makes a prediction, prediction success rate is saved.
{
    std::ifstream infile;
    std::string line;
    infile.open(filename); // Opens txt file

    if (infile.is_open()) // Makes sure the email is open and ready before proceeding
    {
        while (std::getline(infile, line)) // While there are lines to grab
        {
            size_t pos = line.find(','); // Looks for first comma in the line, which signifies that barrier between the email state(spam or not spam) and its content.

            if (pos != std::string::npos)
            {
                std::string label = line.substr(0, pos); // checks whether email is spam or not and saves it.
                std::string content = line.substr(pos + 1);

                SavetoVector(content);        // saves line to vector
                double result = Evaluation(); // evaluates line and makes a prediction.

                if (label == "spam") // checks if the email is spam or not, and edits the count
                {
                    spamEmails2++;
                    spamEmails++;
                    emailState = true;
                }
                else if (label == "notspam")
                {
                    notSpamEmails++;
                    notSpamEmails2++;
                    emailState = false;
                }
                else
                {
                    continue; // Skip if label is not recognized
                }

                // std::cout << "The AI predicts that this email has a spam rating of " << result << std::endl;
                // std::cout << "The true state of this email is " << emailState << std::endl;

                if (emailState == true) // Score keeper
                {
                    if (result > value)
                    {
                        rightGuess++;
                        spamGuess++;
                    }
                    else if (result < value)
                    {
                        wrongGuess++;
                    }
                }
                else if (emailState == false)
                {
                    if (result > value)
                    {
                        wrongGuess++;
                    }
                    else if (result < value)
                    {
                        rightGuess++;
                        nonSpamGuess++;
                    }
                }

                UpdateDatabase(); // Updates database, prepping it for next line.
            }
            else
            {
                std::cerr << "Cannot open file";
            }
        }
    }
    double accuracy = static_cast<double>(rightGuess) / (rightGuess + wrongGuess);
    std::cout << "This AI is " << 100 * accuracy << "% accurate." << std::endl;
    std::cout << "Out of the " << spamEmails2 << " spam emails, it made the proper prediction " << spamGuess << " times." << std::endl;
    std::cout << "Out of the " << notSpamEmails2 << " non spam emails, it made the proper prediction " << nonSpamGuess << " times." << std::endl;
    std::cout << "Out of the " << rightGuess + wrongGuess << " testing emails, it made the proper prediction " << rightGuess << " times." << std::endl;
}

void SupervisedLearning(const std::string &filename)    // Processes email, this function is used for training the AI and adding data to the databases.
{
    std::ifstream infile;
    std::string line;
    infile.open(filename); // Opens txt file

    if (infile.is_open()) // Makes sure the email is open and ready before proceeding
    {
        while (std::getline(infile, line)) // While there are lines to grab
        {
            size_t pos = line.find(','); // Looks for first comma in the line, which signifies that barrier between the email state(spam or not spam) and its content.
            if (pos != std::string::npos)
            {

                std::string label = line.substr(0, pos); // checks whether email is spam or not and saves it.
                std::string content = line.substr(pos + 1);

                SavetoVector(content); // saves line to vector

                if (label == "spam") // checks if the email is spam or not, and edits the count
                {
                    spamEmails++;
                    emailState = true;
                }
                else if (label == "notspam")
                {
                    notSpamEmails++;
                    emailState = false;
                }
                else
                {
                    continue; // Skip if label is not recognized
                }

                UpdateDatabase(); // Updates database, prepping it for next line.
            }
            else
            {
                std::cerr << "Cannot open file";
            }
        }
    }
}

void TestUserInput()    // Tests user input and determines if its spam or not.
{
    int userEmailState = -1;
    std::string testEmail = "";
    std::cout << "Please input an email: " << std::endl;
    std::cin.ignore(); // Clear the input buffer
    std::getline(std::cin, testEmail);

    while (userEmailState != 0 && userEmailState != 1)
    {
        std::cout << "Is this email spam? 1 for yes 0 for no: ";
        std::cin >> userEmailState;

        if (userEmailState != 0 && userEmailState != 1)
        {
            std::cout << "I did not understand your input, please try again." << std::endl;
        }
    }

    SavetoVector(testEmail);
    double result = Evaluation();

    if (result > value)
    {
        std::cout << "The AI predicts that this email is spam." << std::endl;
    }
    else if (result < value)
    {
        std::cout << "The AI predicts that this email is not spam." << std::endl;
    }
    else
    {
        std::cout << "Cannot determine email state. " << std::endl;
    }

    std::cout << "The true state of this email is " << states[userEmailState] << "." << std::endl;
}

void AskUserInput()     // Asks if user would like to input an email
{
    int spamCheckerConsent = -1;

    while (spamCheckerConsent != 0)
    {
        std::cout << "Would you like to input another email? 1 for yes 0 for no: ";
        std::cin >> spamCheckerConsent;
        if (spamCheckerConsent == 0)
        {
            break;
        }
        else
        {
            TestUserInput();
        }
    }
    std::cout << "Goodbye. ";
}

int main()
{
    std::cout << std::endl;
    std::cout << "Intitiating Supervised Learning:" << std::endl;
    SupervisedLearning("C:\\Users\\bobis\\Videos\\New folder\\learning.txt"); // Database file to train AI, 3248 lines of learning, edit the file path
    //Testing("C:\\Users\\bobis\\Videos\\New folder\\learning.txt"); // Database file to test AI, 900 test emails, edit the file path
    std::cout << spamEmails + notSpamEmails << " Emails analyzed and added to database. " << std::endl;
    std::cout << "Supervised Learning Complete:" << std::endl;
    std::cout << std::endl;

    std::cout << "Intitiating AI Testing and Additional Supervised Learning:" << std::endl;
    Testing("C:\\Users\\bobis\\Videos\\New folder\\testing.txt"); // Database file to test AI, 900 test emails, edit the file path
    std::cout << "AI Testing and Additional Supervised Learning Complete:" << std::endl;
    std::cout << std::endl;

    AskUserInput();
}