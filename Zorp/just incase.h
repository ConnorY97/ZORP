//Move the cursor back to the top of the map 
cout << RESTORE_CURSOR_POS;
cout << INDENT << "How tall are you, in centimeters? " << YELLOW << endl << INDENT;

cin >> height;
cout << RESET_COLOR;
if (cin.fail() /*checking if the input is an int*/)
{
	cout << INDENT << "You have failed the first challenge and were eaten by a grue!" << endl;
}
else {
	cout << INDENT << "You entered " << YELLOW << height << RESET_COLOR << endl;
}
//Clear input buffer
cin.clear();
cin.ignore(1000, '\n');
cin.get();

//Move the cursor to the start of the 1st question
cout << RESTORE_CURSOR_POS;
//Delete the next 3 lines of tesxt 
cout << CSI << "3M";
//Insert 3 lines so the maps stays in the same place
cout << CSI << "3L";

cout << INDENT << "What is the first letter of your name? In captial please! " << endl << INDENT << YELLOW;

//cin.clear();
//cin.ignore(1000, '\n');
cin >> firstLetterOfName;
cout << RESET_COLOR;
if (cin.fail() || !isalpha(firstLetterOfName /*Checking if the input is a letter*/))
{
	cout << INDENT << "Youg have failed the second challenge and are eaten by a grue!" << endl;
}
else
{
	cout << INDENT << "You entered " << YELLOW << firstLetterOfName << RESET_COLOR << endl;
}
cin.clear();
cin.ignore(1000, '\n');
cin.get();

cout << RESTORE_CURSOR_POS;
cout << CSI << "A";
cout << CSI << "4M";
cout << CSI << "4L";

if (firstLetterOfName != 0)
{
	avatarHP = (float)height / (firstLetterOfName * 0.02f);
}
else
avatarHP = 0;

cout << endl << INDENT << "Using complex deterministic algorithm, it has been calculated that you have " << GREEN << avatarHP << RESET_COLOR << " hit point(s)" << endl;
