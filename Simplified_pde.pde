
import processing.serial.*;

Serial myPort;  // The serial port

float loadCellData = 0;
color text = #550000;
color liquid = #7895FC;
color value = #000000;
color canister = #ADAEAF;
color can_lid = #D3D3D3;
color canister1Highlight= #F2F5F5;
color canister2Highlight= #F2F5F5;

//canister design
int canister1X = 300;
int canister1Y = 200;
int canister1Width = 540;
int canister1Height = 700;

int canister2X = 1080;
int canister2Y = 200;
int canister2Width = 540;
int canister2Height = 700;

//menu dimensions 
int menuX = 0;
int menuY = 0;
int menuWidth = 330;
int menuHeight = 64;

boolean canister1Over = false;
boolean canister2Over = false;
boolean canister1Menu = false;
boolean canister2Menu = false;
boolean returnToMenu = false; // value for main menu button
int menu = 0;

int xPos = 1;
int lastxPos=1;
int lastHeight=0;

void setup() {
  // List all the available serial ports:
  //printArray(Serial.list());
  // Open the port you are using at the rate you want:
  String portName = "COM4"; 
  myPort = new Serial(this, portName, 9600);
  size(1920,1080);
  frameRate(5);
  println("in setup");
}

void draw() {
  update(mouseX, mouseY); // check mouse location
  
  
  while (myPort.available() > 0) {  //main while loop
    String inBuffer = myPort.readString();
    println("in Draw");
    
  //  if (inBuffer != null) { //only read if not null
      String[] Data = split(inBuffer, ",");
      loadCellData = float(inBuffer);
      
      
      try{
      
        println(Data[0]);
        println(Data[1]);
        
        float loadCellData_1 = float(Data[0]);
        float loadCellData_2 = float(Data[1]);
        float inByteMapped_1 = map(loadCellData_1, 0,1023, 0, height);
        float inByteMapped_2 = map(loadCellData_2, 0,1023, 0, height);
        
        switch(menu){
           case 0:
             background(100);
             //println("Main Menu");
              drawCanisters();
              drawReturnToMenu();
              highLightCanister1(canister1Over);
              highLightCanister2(canister2Over);
              liquidAnimation(loadCellData_1);
              liquidAnimation_2(loadCellData_2);
              
              
             break;
           case 1:
            // println("Canister 1");
             background(255);
             drawReturnToMenu();
             //graphCanister1(inByteMapped_1);
             
             
             
             break;
           case 2:
            // println("Canister 2");
             background(150);
             drawReturnToMenu();
             break;
       } 
      
        
        
      }
      // if bad data is passed, pass a zero value.
      catch(ArrayIndexOutOfBoundsException e) {
        e.printStackTrace();
        float loadCellData_1 = 0.0;
        float loadCellData_2 = 0.0;
      }
      
      
      //LoadCellData ;
      //println(loadCellData);
      
      //drawCanisters();

      
    }
 // }
}


void drawCanisters(){
  //canister 1
  fill(canister);
  rect(canister1X, canister1Y, canister1Width, canister1Height, 100, 100, 20 ,20);//outer body
  //rect(920, 160, 180, 40, 100,100,0,0);// lid
  //fill(can_lid);
  //rect(canister1X + 20, canister1Y + 20, canister1Width-40, canister1Height-40, 100, 100, 0, 0);// inner body
  textSize(64);
  fill(#47E6EA);
  text("EBR", 500, 200);

  //canister 2 
  fill(canister);
  rect(canister2X, canister2Y , canister2Width, canister2Height, 100, 100, 20 ,20);//outer body
  //rect(920, 160, 180, 40, 100,100,0,0);// lid
  fill(can_lid);
  rect(canister2X + 20, canister2Y + 20, canister2Width-40, canister2Height-40, 100, 100, 0, 0);// inner body
  fill(#1007A5);
  text("OPD 4262", 1200,200);
  fill(#0D00D8);
}

void drawReturnToMenu(){
  fill(#EA0000);
  rect(menuX,menuY,menuWidth,menuHeight);
  fill(0);
  textSize(64);
  text("Main Menu", 20, 50);
}


void update(int x, int y){
  
  if (overCanister(canister1X,canister1Y,canister1Width,canister1Height)){
    canister1Over = true;
    canister2Over = false;
  }
  
  else if (overCanister(canister2X,canister2Y,canister2Width,canister2Height)){
    canister2Over = true;
    canister1Over = false;
    
  }
  else if (returnToMenu(menuX, menuY, menuWidth, menuHeight)){
    returnToMenu = true;
  
  }
  
  else {
    canister1Over = false;
    canister2Over = false;
    returnToMenu = false;
  }
}

//checks if mouse is over canister
boolean overCanister(int x, int y, int width, int height){
  if (mouseX >= x && mouseX <= x+width && mouseY >= y && mouseY <= y+height){
    return true;
  }
  else{
    return false;
  }
  
}
boolean returnToMenu(int x, int y, int width, int height){
  if (mouseX >= x && mouseX <= x+width && mouseY >= y && mouseY <= y+height){
    return true;
  }
  else{
    return false;
  }

}

void mousePressed() {
  if (canister1Over) {
    canister1Menu = true;
    menu = 1;
  }
   else if (canister2Over) {
    canister2Menu = true;
    menu = 2;
  }
  else if (returnToMenu){
    menu = 0;
  }
  else{
    canister1Menu = false;
    canister2Menu = false;
    //menu = 0;
  }
}

//not working
/*
void graphCanister1(float i){
  
    stroke(127,34,255);     //stroke color
    strokeWeight(4);        //stroke wider
    line(lastxPos, lastHeight, xPos, height - i);
    lastxPos = xPos;
    lastHeight = int(height - i);
    
    if (xPos >= width){
      xPos = 0;
      lastxPos = 0;
      background(0);
    }
    
    else{
      xPos++;
  }
}
*/

void liquidAnimation(float i){
  // 700 pixel height 
  //scaling liquid
  
  float scaling = ((i-30)/30);
  float percent = scaling *100;
  percent = round(percent);
  
  i = int(i);
  
  if (i <= 35){ //empty canister
  } 
  else if (percent >= 0 && i <=99 ){
    fill(liquid); 
    rect(canister1X+20,880, 500, scaling*-600);
    println(scaling);
    println(i);
    fill(#5A5A5A);
    text(round(i) + "lbs", 400,800);
    text(round(percent) + "%",400,850);
    if( percent >= 0 && percent <= 40){
      fill(#FF0808);
      text("LOW!", 500,700);
    }
    else if (percent >=40 && percent <= 60){
      fill(#F8FC03);
      text("Refill Required", 400,700);
    }
    else if (percent >= 60 && percent <= 99){
      fill(#00FC0A);
      text("Good", 500,700);
    }
    
    
  }
  else if (percent >= 100){
    fill(liquid); 
    rect(canister1X + 20, canister1Y + 20, canister1Width-40, canister1Height-40, 100, 100, 0, 0);// inner body
    fill(#00FC0A);
    text("Full", 500,700);
    text(100 + "%",400,850);
  }
  //i = (i - 35) * -20;
  
  //rect(canister1X+20,880,500,0+i);

}


void liquidAnimation_2(float i){
  // 700 pixel height 
  //scaling liquid
  
  float scaling = ((i-30)/30);
  float percent = scaling *100;
  percent = round(percent);
  
  i = int(i);
  
  if (i <= 35){ //empty canister
  } 
  else if (percent >= 0 && i <=99 ){
    fill(liquid); 
    rect(canister2X+20,880, 500, scaling*-600);
    println(scaling);
    println(i);
    fill(#5A5A5A);
    text(round(i) + "lbs", 1150,800);
    text(round(percent) + "%",1150,850);
    if( percent >= 0 && percent <= 40){
      fill(#FF0808);
      text("LOW!", 1150,700);
    }
    else if (percent >=40 && percent <= 60){
      fill(#F8FC03);
      text("Refill Required", 1150,700);
    }
    else if (percent >= 60 && percent <= 99){
      fill(#00FC0A);
      text("Good", 1150,700);
    }
    
    
  }
  else if (percent >= 100){
    fill(liquid); 
    rect(canister2X + 20, canister2Y + 20, canister2Width-40, canister2Height-40, 100, 100, 0, 0);// inner body
    fill(#00FC0A);
    text("Full", 1150,700);
    text(100 + "%",1150,850);
  }
  //i = (i - 35) * -20;
  
  //rect(canister1X+20,880,500,0+i);

}




void highLightCanister1(boolean canister1Over){
  if (canister1Over){
            fill(canister1Highlight);
          }
          else{
            fill(can_lid);
          }
         rect(canister1X + 20, canister1Y + 20, canister1Width-40, canister1Height-40, 100, 100, 0, 0);// inner body
         
  
}


void highLightCanister2(boolean canister2Over){
  if (canister2Over){
            fill(canister2Highlight);
          }
          else{
            fill(can_lid);
          }
         rect(canister2X + 20, canister2Y + 20, canister2Width-40, canister2Height-40, 100, 100, 0, 0);// inner body
         
  
}
