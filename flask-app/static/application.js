
$(document).ready(function(){
    // Connect to the socket server.
    var socket = io.connect('http://' + document.domain + ':' + location.port + '/test');

    // Receive details from server
    socket.on('newlocation', function(msg) {
        console.log("Received - latitude: " + msg.latitude);
        console.log("Received - longitude: " + msg.longitude);
    
        // Display the new values, replacing the previous ones
        $('#log1').html(msg.latitude.toString());
        $('#log2').html(msg.longitude.toString());
        $('#log3').html(msg.latitude.toString());
        $('#log4').html(msg.longitude.toString());
        $('#log5').html(msg.latitude.toString());
        $('#log6').html(msg.longitude.toString());
    });
    
});


// $(document).ready(function(){
//     //connect to the socket server.
//     var socket = io.connect('http://' + document.domain + ':' + location.port + '/test');
//     var numbers_received = [];

//     //receive details from server
//     socket.on('newnumber', function(msg) {
//         console.log("Received number" + msg.number);
//         //maintain a list of ten numbers
//         if (numbers_received.length >= 1){
//             numbers_received.shift()
//         }            
//         numbers_received.push(msg.number);
//         numbers_string = '';
//         for (var i = 0; i < numbers_received.length; i++){
//             numbers_string = numbers_string + '<p>' + numbers_received[i].toString() + '</p>';
//         }
//         $('#log').html(numbers_string);
//     });

// });

