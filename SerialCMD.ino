#define DEBUGGING true		//set to false to disable verbose output
String inputString = "";
char inputChar; // Where to store the character read

void setup() {
    while (!Serial); //delay for Micro/Leonardo boards
    Serial.begin(115200);
    
    Serial.println("Test started");
}

void loop(){
   //Serial.available does not return more than 1 on VISduinoboards (and all other boards with cheap USB to Serial chips)
   while (Serial.available() > 0){
      inputChar = Serial.read(); // Read character
      if(inputChar == ')'){
        inputString += inputChar;
        Serial.print("String: ");Serial.println(inputString);
        processInputCommand(inputString);
        inputString = "";
      }
      else
        inputString += inputChar; // Add character to string
  }    
}

int processInputCommand(String inputString){
  int beginBracket = -1;
  int endBracket = -1;
  int commas[9];
  int nextCommaIndex = 0;
  int parameters[10];
  
  for(int i = 0; i < inputString.length(); i++){    
    if(inputString.charAt(i) == '(')
      if(beginBracket == -1)
        beginBracket = i;
      else
        return 1; //two ( found

    if(inputString.charAt(i) == ')')
      if(endBracket == -1)   
        endBracket = i;
      else
        return 2; //two ) found

    if(inputString.charAt(i) == ','){
      //Serial.print("comma found at ");  Serial.println(i);
      commas[nextCommaIndex] = i;
      if(nextCommaIndex < 9)
        nextCommaIndex++;
      else{
        Serial.print("too many commas at");  Serial.println(i);
        }
    } 
  }
      
  if(DEBUGGING){
    Serial.print("\nbeginBracket: ");  Serial.println(beginBracket);
    Serial.print("endBracket: ");  Serial.println(endBracket);
  }

  if(beginBracket == -1)
    return 3; //no ( found
  if(endBracket == -1)
    return 4; //no ) found

  if(DEBUGGING)
    for(int i = 0; i < nextCommaIndex; i++){
      Serial.print("Comma    ");  Serial.print(i);  Serial.print (" : "); Serial.println(commas[i]);
    }


  for(int i = 0; i < nextCommaIndex; i++){
    switch (i){
      case 0:
        parameters[0] = inputString.substring(beginBracket +1, commas[0]).toInt();
        if(DEBUGGING){
          Serial.print("Argument    ");  Serial.print(0);  Serial.print (" : "); Serial.println( parameters[0]);  }
      break;
      
      default:
        parameters[i] = inputString.substring(commas[i-1]+1, commas[i]).toInt();
        if(DEBUGGING){
          Serial.print("Argument    ");  Serial.print(i);  Serial.print (" : "); Serial.println(parameters[i]); }
    }
  }

  parameters[nextCommaIndex] = inputString.substring(commas[ nextCommaIndex-1]+1, endBracket).toInt();
  if(DEBUGGING){
    Serial.print("Argument    ");  
    Serial.print( nextCommaIndex);  
    Serial.print (" : "); 
    Serial.println( parameters[nextCommaIndex] ); 
  }  
  
  String methode = inputString.substring(0, beginBracket);
  if(DEBUGGING){
    Serial.print("Methode:   ");  Serial.println(methode);          //DEBUGGING
    Serial.println();
  }  
}



