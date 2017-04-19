$(document).ready(function(){
	var name = "QR=";
	var c = document.cookie;
	if (c.substring(name.length,c.length)=="yes"){
		$("a#QRc").css("display","block");
		$("img#QRimage").css("display","none");
		$("a#hideQRc").css("display","none");
	}
	if (c.substring(name.length,c.length)=="no"){
		$("a#QRc").css("display","none");
		$("img#QRimage").css("display","block");
		$("p#hideQRc").css("display","block");
	}
	$( "li.dropdown" ).click(function() {
		$(this).children("ul.sub-menu" ).slideToggle( "slow", function() {

		});
	});
	$( "li.dropdown" ).hover(function() {
		$(this).children("ul.sub-menu" ).slideUp( "slow", function() {

		});
	});
	$( "img.photos" ).click(function() {
        $("img#main").attr("src", $(this).attr("src"));
    });
    $( "a#QRc" ).click(function() {
    	document.cookie = "QR=no";
        $(this).hide(),
        $("p#hideQRc").show(),
        $("img#QRimage").show();
    });
    $( "img#QRimage" ).click(function() {
    	document.cookie = "QR=yes";
        $(this).hide(),
        $("p#hideQRc").hide(),
        $("a#QRc").show();
    });
});