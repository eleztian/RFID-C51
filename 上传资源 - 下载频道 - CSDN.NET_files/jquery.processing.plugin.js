
(function($) {
	$.fn.progressDialog = function() {

	};

	$.fn.progressDialog.showDialog = function(target_div) {
		loadDiv = target_div;
		createElement();
		waterfall.appendTo("body");
	}

	$.fn.progressDialog.hideDialog = function(text) {
		waterfall.remove();
	}

	function createElement() {
		if (!waterfall) {
			var width = document.body.clientWidth;
			var height = document.body.clientHeight+30;
			
			waterfall = $(document.createElement("div"));
			waterfall.attr("id", "waterfall");
			waterfall.css( {
				"height" : height,
				"width" : width,
				"filter" : "alpha(opacity = 50)",
				"-moz-opacity" : "0.5",
				"opacity" : "0.5",
				"background-color" : "#CCCCCC",
				"position" : "absolute",
				"left" : "0px",
				"top" : "0px"
			});
		}
		if (!loadDiv) {
			var text = "Loading,Please wait...";
			loadDiv = document.createElement("div");
			$(loadDiv).appendTo(waterfall);
			
			var content = " <div style='width:" +width+ "px; height:" +Height+ "px;'><div style='width:100%; height:30px; line-height:31px;padding-left:15px;font-weight:bolder; color:#929292;'>"+text+"</div><div align='center'><img src='loading.gif' border='0'/></div></div>";
			$(loadDiv).html(content);
		}else{
			$(loadDiv).css('display','block');
		}
		
	}

	function setPosition() {
		var leftOffset = ($(document).width() - width) / 2;
		var topOffset = ($(document).height() - Height) / 2;
		$(loadDiv).css( {
			"position" : "absolute",
			"height" : Height + "px",
			"width" : width + "px",
			"left" : leftOffset + "px",
			"top" : topOffset + "px"
		});
	}

	var waterfall;
	var loadDiv;
	var width = 290;
	var Height = 60;
})(jQuery);