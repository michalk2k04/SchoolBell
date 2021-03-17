setInterval(function ( ) 
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() 
  {
  
    if (this.readyState == 4 && this.status == 200)
    {
      document.getElementById("nextBell").innerHTML = this.responseText;
    }
  };
  
  xhttp.open("GET", "/nextBell", true);
  xhttp.send();
  
}, 1000 ) ;

setInterval(function ( ) 
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() 
  {
  
    if (this.readyState == 4 && this.status == 200)
    {
      document.getElementById("time").innerHTML = this.responseText;
    }
  };
  
  xhttp.open("GET", "/getTime", true);
  xhttp.send();
  
}, 500 ) ;