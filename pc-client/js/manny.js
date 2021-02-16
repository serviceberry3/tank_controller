//if any button from the ".button" class is clicked 
$(".button").click(function () {   
    //get this button id and store it inside the "p" variable.                                
    var p = $(this).attr('id');       

    //a dictionary contains the button id to get sent to the web server. 
    pin: p                           
                
    //then send a get request to the web server"http://192.168.1.4:80/" (":80" means port number 80) with some data in a form 
    //of dictionary {pin: p} which is the button id
    $.get("http://192.168.1.4:80/", {                           
        pin: p      
    });      
}); 