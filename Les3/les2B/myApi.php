<?php
        
        
        // Report all errors
        // Dit is nu handig bij het testen
        // In een productie omgeving doe je dit niet of anders
        error_reporting(E_ALL);


        
        // my API reageert op GET en POST requests
        print "<h3>hello from myAPI !!</h3>"; // say hello to client
     
        
        $myTime = time(); // get the server time
        $IP = getHostByName(getHostName()); // get local ip

		if (!empty($_GET['data'])){  
            $dataFromGet= $_GET['data'];
            print "<h4>GET request ontvangen</h4>";
            print "de data uit het GET request is: " . $dataFromGet;
            switch($dataFromGet){
                case "server":
                        print "<pre>my server variables are:  " ;
                        print_r($_SERVER);  // show server data
                        print "</pre>"; 
                    break;
                case "tijd":
                        print "<br>server tijd: " . date("h:i:s",$myTime);
                    break;

                 // hier moet jij zelf code schrijven
                case "ip":
                        print "<br>IP: " . $IP;
                    break;
                case "tijd ip":
                        print "<br>server tijd: " . date("h:i:s",$myTime) . "<br>" . "IP:" . $IP;
                    break;
                  // einde eigen code  
                default:
                    print "<h3>please read the manual for valid requests &#128540;<br>Gebruik: ip/tijd ip/tijd/server</h3>";
            }
        }     
?>
