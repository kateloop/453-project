   //note length defines
   #define br   2    //break length
    #define en  10  //eighth note
    #define den 15  //dotted eighth note
    #define qn  20  //quarter note
    #define dqn 30  //dotted quarter note
    #define hn  40  //half note
    #define dhn 50  //dotted half note
    #define wn  80  //whole note

    
    //sets channel, plays note, toggles LEDs
    void getNote(unsigned char uNote){
        if (uNote == UA3){
            conv_result.result = UA3;
            conv_result.adc_channel = 0;
            play(UA3);
            ToggleLeds();
        }
        else if (uNote == UA3F){
            conv_result.result = UA3F;
            conv_result.adc_channel = 0;
            play(UA3F);
            ToggleLeds();
        }
        else if (uNote == UA3S){
            conv_result.result = UA3S;
            conv_result.adc_channel = 0;
            play(UA3S);
            ToggleLeds();
        }
        else if (uNote == UB3){
            conv_result.result = UB3;
            conv_result.adc_channel = 1;
            play(UB3);
            ToggleLeds();
        }
        else if (uNote == UB3F){
            conv_result.result = UB3F;
            conv_result.adc_channel = 1;
            play(UB3F);
            ToggleLeds();
        }
        else if (uNote == UB3S){
            conv_result.result = UB3S;
            conv_result.adc_channel = 1;
            play(UB3S);
            ToggleLeds();
        }
        else if (uNote == UC3){
            conv_result.result = UC3;
            conv_result.adc_channel = 2;
            play(UC3);
            ToggleLeds();
        }
        else if (uNote == UC3F){
            conv_result.result = UC3F;
            conv_result.adc_channel = 2;
            play(UC3F);
            ToggleLeds();
        }
        else if (uNote == UC3S){
            conv_result.result = UC3S;
            conv_result.adc_channel = 2;
            play(UC3S);
            ToggleLeds();
        }
        else if (uNote == UD3){
            conv_result.result = UD3;
            conv_result.adc_channel = 3;
            play(UD3);
            ToggleLeds();
        }
        else if (uNote == UD3F){
            conv_result.result = UD3F;
            conv_result.adc_channel = 3;
            play(UD3F);
            ToggleLeds();
        }
        else if (uNote == UD3S){
            conv_result.result = UD3S;
            conv_result.adc_channel = 3;
            play(UD3S);
            ToggleLeds();
        }
        else if (uNote == UE3){
            conv_result.result = UE3;
            conv_result.adc_channel = 4;
            play(UE3);
            ToggleLeds();
        }
        else if (uNote == UE3F){
            conv_result.result = UE3F;
            conv_result.adc_channel = 4;
            play(UE3F);
            ToggleLeds();
        }
        else if (uNote == UE3S){
            conv_result.result = UE3S;
            conv_result.adc_channel = 4;
            play(UE3S);
            ToggleLeds();
        }
        else if (uNote == UF3){
            conv_result.result = UF3;
            conv_result.adc_channel = 5;
            play(UF3);
            ToggleLeds();
        }
        else if (uNote == UF3F){
            conv_result.result = UF3F;
            conv_result.adc_channel = 5;
            play(UF3F);
            ToggleLeds();
        }
        else if (uNote == UF3S){
            conv_result.result = UF3S;
            conv_result.adc_channel = 5;
            play(UF3S);
            ToggleLeds();
        }
        else if (uNote == UG3){
            conv_result.result = UG3;
            conv_result.adc_channel = 6;
            play(UG3);
            ToggleLeds();
        }
        else if (uNote == UG3F){
            conv_result.result = UG3F;
            conv_result.adc_channel = 6;
            play(UG3F);
            ToggleLeds();
        }
        else if (uNote == UG3S){
            conv_result.result = UG3S;
            conv_result.adc_channel = 6;
            play(UG3S);
            ToggleLeds();
        }
        else if (uNote == UA4){
            conv_result.result = UA4;
            conv_result.adc_channel = 0;
            play(UA4);
            ToggleLeds();
        }
        else if (uNote == UA4F){
            conv_result.result = UA4F;
            conv_result.adc_channel = 0;
            play(UA4F);
            ToggleLeds();
        }
        else if (uNote == UA4S){
            conv_result.result = UA4S;
            conv_result.adc_channel = 0;
            play(UA4S);
            ToggleLeds();
        }
        else if (uNote == UB4){
            conv_result.result = UB4;
            conv_result.adc_channel = 1;
            play(UB4);
            ToggleLeds();
        }
        else if (uNote == UB4F){
            conv_result.result = UB4F;
            conv_result.adc_channel = 1;
            play(UB4F);
            ToggleLeds();
        }
        else if (uNote == UB4S){
            conv_result.result = UB4S;
            conv_result.adc_channel = 1;
            play(UB4S);
            ToggleLeds();
        }
        else if (uNote == UC4){
            conv_result.result = UC4;
            conv_result.adc_channel = 2;
            play(UC4);
            ToggleLeds();
        }
        else if (uNote == UC4F){
            conv_result.result = UC4F;
            conv_result.adc_channel = 2;
            play(UC4F);
            ToggleLeds();
        }
        else if (uNote == UC4S){
            conv_result.result = UC4S;
            conv_result.adc_channel = 2;
            play(UC4S);
            ToggleLeds();
        }
        else if (uNote == UD4){
            conv_result.result = UD4;
            conv_result.adc_channel = 3;
            play(UD4);
            ToggleLeds();
        }
        else if (uNote == UD4F){
            conv_result.result = UD4F;
            conv_result.adc_channel = 3;
            play(UD4F);
            ToggleLeds();
        }
        else if (uNote == UD4S){
            conv_result.result = UD4S;
            conv_result.adc_channel = 3;
            play(UD4S);
            ToggleLeds();
        }
        else if (uNote == UE4){
            conv_result.result = UE4;
            conv_result.adc_channel = 4;
            play(UE4);
            ToggleLeds();
        }
        else if (uNote == UE4F){
            conv_result.result = UE4F;
            conv_result.adc_channel = 4;
            play(UE4F);
            ToggleLeds();
        }
        else if (uNote == UE4S){
            conv_result.result = UE4S;
            conv_result.adc_channel = 4;
            play(UE4S);
            ToggleLeds();
        }
        else if (uNote == UF4){
            conv_result.result = UF4;
            conv_result.adc_channel = 5;
            play(UF4);
            ToggleLeds();
        }
        else if (uNote == UF4F){
            conv_result.result = UF4F;
            conv_result.adc_channel = 5;
            play(UF4F);
            ToggleLeds();
        }
        else if (uNote == UF4S){
            conv_result.result = UF4S;
            conv_result.adc_channel = 5;
            play(UF4S);
            ToggleLeds();
        }
        else if (uNote == UG4){
            conv_result.result = UG4;
            conv_result.adc_channel = 6;
            play(UG4);
            ToggleLeds();
        }
        else if (uNote == UG4F){
            conv_result.result = UG4F;
            conv_result.adc_channel = 6;
            play(UG4F);
            ToggleLeds();
        }
        else if (uNote == UG4S){
            conv_result.result = UG4S;
            conv_result.adc_channel = 6;
            play(UG4S);
            ToggleLeds();
        }
        else if (uNote == UA5){
            conv_result.result = UA5;
            conv_result.adc_channel = 0;
            play(UA5);
            ToggleLeds();
        }
        else if (uNote == UA5F){
            conv_result.result = UA5F;
            conv_result.adc_channel = 0;
            play(UA5F);
            ToggleLeds();
        }
        else if (uNote == UA5S){
            conv_result.result = UA5S;
            conv_result.adc_channel = 0;
            play(UA5S);
            ToggleLeds();
        }
        else if (uNote == UB5){
            conv_result.result = UB5;
            conv_result.adc_channel = 1;
            play(UB5);
            ToggleLeds();
        }
        else if (uNote == UB5F){
            conv_result.result = UB5F;
            conv_result.adc_channel = 1;
            play(UB5F);
            ToggleLeds();
        }
        else if (uNote == UB5S){
            conv_result.result = UB5S;
            conv_result.adc_channel = 1;
            play(UB5S);
            ToggleLeds();
        }
        else if (uNote == UC5){
            conv_result.result = UC5;
            conv_result.adc_channel = 2;
            play(UC5);
            ToggleLeds();
        }
        else if (uNote == UC5F){
            conv_result.result = UC5F;
            conv_result.adc_channel = 2;
            play(UC5F);
            ToggleLeds();
        }
        else if (uNote == UC5S){
            conv_result.result = UC5S;
            conv_result.adc_channel = 2;
            play(UC5S);
            ToggleLeds();
        }
        else if (uNote == UD5){
            conv_result.result = UD5;
            conv_result.adc_channel = 3;
            play(UD5);
            ToggleLeds();
        }
        else if (uNote == UD5F){
            conv_result.result = UD5F;
            conv_result.adc_channel = 3;
            play(UD5F);
            ToggleLeds();
        }
        else if (uNote == UD5S){
            conv_result.result = UD5S;
            conv_result.adc_channel = 3;
            play(UD5S);
            ToggleLeds();
        }
        else if (uNote == UE5){
            conv_result.result = UE5;
            conv_result.adc_channel = 4;
            play(UE5);
            ToggleLeds();
        }
        else if (uNote == UE5F){
            conv_result.result = UE5F;
            conv_result.adc_channel = 4;
            play(UE5F);
            ToggleLeds();
        }
        else if (uNote == UE5S){
            conv_result.result = UE5S;
            conv_result.adc_channel = 4;
            play(UE5S);
            ToggleLeds();
        }
        else if (uNote == UF5){
            conv_result.result = UF5;
            conv_result.adc_channel = 5;
            play(UF5);
            ToggleLeds();
        }
        else if (uNote == UF5F){
            conv_result.result = UF5F;
            conv_result.adc_channel = 5;
            play(UF5F);
            ToggleLeds();
        }
        else if (uNote == UF5S){
            conv_result.result = UF5S;
            conv_result.adc_channel = 5;
            play(UF5S);
            ToggleLeds();
        }
        else if (uNote == UG5){
            conv_result.result = UG5;
            conv_result.adc_channel = 6;
            play(UG5);
            ToggleLeds();
        }
        else if (uNote == UG5F){
            conv_result.result = UG5F;
            conv_result.adc_channel = 6;
            play(UG5F);
            ToggleLeds();
        }
        else if (uNote == UG5S){
            conv_result.result = UG5S;
            conv_result.adc_channel = 6;
            play(UG5S);
            ToggleLeds();
        }
        
    }

    //playing Piano Man by Billy Joel
    void playPianoMan(){
        //verse 
        getNoteTeachMode(UG4);
        DelayMs(qn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UG4);
        DelayMs(hn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UG4);
        DelayMs(qn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UG4);
        DelayMs(qn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UF4);
        DelayMs(dqn);
        getNoteTeachMode(UE4);
        DelayMs(en);
        getNoteTeachMode(UF4);
        DelayMs(en);
        getNoteTeachMode(UE4);
        DelayMs(en);
        getNoteTeachMode(UC4);
        DelayMs(hn);
        getNoteTeachMode(0);
        DelayMs(hn);
        getNoteTeachMode(UC4);
        DelayMs(qn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UC4);
        DelayMs(qn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UC4);
        DelayMs(dqn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UC4);
        DelayMs(en);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UC4);
        DelayMs(qn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UC4);
        DelayMs(en);
        getNoteTeachMode(UD4);
        DelayMs(dqn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UD4);
        DelayMs(wn);
        getNoteTeachMode(0);
        DelayMs(qn);
        getNoteTeachMode(UG4);
        DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG4);
      DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG4);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG4);
      DelayMs(wn);
        getNoteTeachMode(UF4);
        DelayMs(en);
        getNoteTeachMode(UE4);
        DelayMs(en);
        getNoteTeachMode(UF4);
        DelayMs(qn);
        getNoteTeachMode(UE4);
        DelayMs(en);
        getNoteTeachMode(UC4);
        DelayMs(en);
        DelayMs(dhn);
        getNoteTeachMode(UA3);
        DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UA3);
      DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UA3);
      DelayMs(hn);
        getNoteTeachMode(UF4);
        DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UF4);
      DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UF4);
      DelayMs(qn);
        getNoteTeachMode(UE4);
        DelayMs(qn);
        getNoteTeachMode(UC4);
        DelayMs(qn);
        getNoteTeachMode(UC4);
        DelayMs(dhn);
        play(0);
        DelayMs(wn);
        DelayMs(hn);
        DelayMs(dqn);
        getNoteTeachMode(UG5);
        DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(qn);
        getNoteTeachMode(UF5);
        DelayMs(qn);
        getNoteTeachMode(UE5);
        DelayMs(qn);
        getNoteTeachMode(UF5);
        DelayMs(en);
        getNoteTeachMode(UE5);
        DelayMs(en);
        getNoteTeachMode(UC5);
        DelayMs(wn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(hn);
        getNoteTeachMode(UD5);
        DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UD5);
      DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UD5);
      DelayMs(wn);
        getNoteTeachMode(0);
        DelayMs(qn);
        getNoteTeachMode(UE5);
        DelayMs(en);
        getNoteTeachMode(UF5);
        DelayMs(en);
        getNoteTeachMode(UG5);
        DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(dqn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(hn);
        getNoteTeachMode(UF5);
        DelayMs(en);
        getNoteTeachMode(UE5);
        DelayMs(en);
        getNoteTeachMode(UF5);
        DelayMs(qn);
        getNoteTeachMode(UE5);
        DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UE5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UE5);
      DelayMs(hn);
        getNoteTeachMode(UC5);
        DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(dqn);
        getNoteTeachMode(UE5);
        DelayMs(en);
        getNoteTeachMode(UF5);
        DelayMs(dqn);
        getNoteTeachMode(UE5);
        DelayMs(en);
        getNoteTeachMode(UC5);
        DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(wn);
        DelayMs(hn);
        //chorus
        //"Da-da's"
        getNoteTeachMode(UA4);
        DelayMs(hn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UA4);
      DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UA4);
      DelayMs(dhn);
        getNoteTeachMode(UB4);
        DelaynMs(en);
        getNoteTeachMode(UC5);
        DelayMs(en);
        getNoteTeachMode(UB4);
        DelayMs(en);
        getNoteTeachMode(UA4);
        DelayMs(hn);
        DelayMs(en);
        getNoteTeachMode(0);
        DelayMs(wn);
        DelayMs(qn);
        getNoteTeachMode(UA4);
        DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UA4);
      DelayMs(en);
        DelayMs(hn);
        getNoteTeachMode(UB4);
        DelayMs(en);
        getNoteTeachMode(UC5);
        DelayMs(en);
        getNoteTeachMode(UB4);
        DelayMs(wn);
        DelayMs(qn);
        getNoteTeachMode(UA4);
        DelayMs(en);
        getNoteTeachMode(UG4);
        DelayMs(en);
        DelayMs(dhn);
        getNoteTeachMode(0);
        DelayMs(wn);
        DelayMs(wn);
        //"Sing us a song, you're the piano man..."
        getNoteTeachMode(UG5);
        DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(dqn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(qn);
        getNoteTeachMode(UF5);
        DelayMs(dqn);
        getNoteTeachMode(UE5);
        DelayMs(en);
        getNoteTeachMode(UF5);
        DelayMs(en);
        getNoteTeachMode(UE5);
        DelayMs(qn);
        getNoteTeachMode(UC5);
        DelayMs(dqn);
        DelayMs(dhn);
        getNoteTeachMode(UA4);
        DelayMs(qn);
        getNoteTeachMode(UC5);
        DelayMs(dqn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UC5);
        DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(hn);
        getNoteTeachMode(UD5);
        DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UD5);
      DelayMs(wn);
        getNoteTeachMode(UE5);
        DelayMs(qn);
        getNoteTeachMode(UF5);
        DelayMs(qn);
        getNoteTeachMode(UG5);
        DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(dqn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(qn);
        getNoteTeachMode(UF5);
        DelayMs(dqn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UF5);
      DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UF5);
      DelayMs(en);
        getNoteTeachMode(UE5);
        DelayMs(en);
        getNoteTeachMode(UC5);
        DelayMs(hn);
        getNoteTeachMode(0);
        DelayMs(hn);
        getNoteTeachMode(UG4);
        DelayMs(qn);
        getNoteTeachMode(UA4);
        DelayMs(qn);
        getNoteTeachMode(UC5);
        DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(qn);
        getNoteTeachMode(UF5);
        DelayMs(qn);
        getNoteTeachMode(UE5);
        DelayMs(qn);
        getNoteTeachMode(UC5);
        DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(wn);
        DelayMs(hn);
    }

    void playClocks(){
        while(1){
            getNoteTeachMode(UE5F);
            DelayMs(en);
            getNoteTeachMode(UB4F);
            DelayMs(en);
            getNoteTeachMode(UG4);
            DelayMs(en);
            getNoteTeachMode(UE5F);
            DelayMs(en);
            getNoteTeachMode(UB4F);
            DelayMs(en);
            getNoteTeachMode(UG4);
            DelayMs(en);
            getNoteTeachMode(UE5F);
            DelayMs(en);
            getNoteTeachMode(UB4F);
            DelayMs(en);
            getNoteTeachMode(UD5F);
            DelayMs(en);
            getNoteTeachMode(UB4F);
            DelayMs(en);
            getNoteTeachMode(UF4);
            DelayMs(en);
            getNoteTeachMode(UD5F);
            DelayMs(en);
            getNoteTeachMode(UB4F);
            DelayMs(en);
            getNoteTeachMode(UF4);
            DelayMs(en);
            getNoteTeachMode(UD5F);
            DelayMs(en);
            getNoteTeachMode(UB4F);
            DelayMs(en);
            getNoteTeachMode(UD5F);
         DelayMs(en);
         getNoteTeachMode(UB4F);
         DelayMs(en);
         getNoteTeachMode(UF4);
         DelayMs(en);
         getNoteTeachMode(UD5F);
         DelayMs(en);
         getNoteTeachMode(UB4F);
         DelayMs(en);
         getNoteTeachMode(UF4);
         DelayMs(en);
         getNoteTeachMode(UD5F);
         DelayMs(en);
         getNoteTeachMode(UB4F);
         DelayMs(en);
            getNoteTeachMode(UC5);
            DelayMs(en);
            getNoteTeachMode(UA4F);
            DelayMs(en);
            getNoteTeachMode(UF4);
            DelayMs(en);
            getNoteTeachMode(UC5);
            DelayMs(en);
            getNoteTeachMode(UA4F);
            DelayMs(en);
            getNoteTeachMode(UF4);
            DelayMs(en);
            getNoteTeachMode(UC5);
            DelayMs(en);
            getNoteTeachMode(UA4F);
            DelayMs(en);
    }
}

    void playClair(){
        getNoteTeachMode(UA4F);
        DelayMs(en);
        getNoteTeachMode(UA5F);
        DelayMs(en);
        DelayMs(dqn);
        getNoteTeachMode(UF5);
        DelayMs(hn);
        getNoteTeachMode(UE5F);
        DelayMs(en);
        getNoteTeachMode(UF5);
        DelayMs(en);
        getNoteTeachMode(UE5F):
        DelayMs(dhn);
        DelayMs(en);
        getNoteTeachMode(UD5F);
        DelayMs(en):
        getNoteTeachMode(UE5F);
        DelayMs(en);
        getNoteTeachMode(UD5F);
        DelayMs(en);
        getNoteTeachMode(UF5);
        DelayMs(den);
        getNoteTeachMode(UD5F);
        DelayMs(den);
        getNoteTeachMode(UC5);
        DelayMs(en);
        getNoteTeachMode(UD5F);
        DelayMs(en);
        getNoteTeachMode(UC5);
        DelayMs(dhn);
        DelayMs(en);
        getNoteTeachMode(UB4F);
        DelayMs(en);
        getNoteTeachMode(UC5);
        DelayMs(en);
        getNoteTeachMode(UB4F);
        DelayMs(en);
        getNoteTeachMode(UE5F);
        DelayMs(en);
        getNoteTeachMode(UB4F);
        DelayMs(en);
        getNoteTeachMode(UA4F);
        DelayMs(en);
        getNoteTeachMode(UB4F);
        DelayMs(en);
        getNoteTeachMode(UA4F);
        DelayMs(qn);
        getNoteTeachMode(UG4F);
        DelayMs(en);
        getNoteTeachMode(UA4F);
        DelayMs(en);
        getNoteTeachMode(UG4F);
        Delay(dqn);
        
    }

