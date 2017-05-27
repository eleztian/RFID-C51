
 
(function($) {
	var load_classes_xml = function(callback){
		if(load_classes_xml.xml_obj == undefined){
			$.ajax({url:'/js/classes.xml', async:false, success:function(data) {
				load_classes_xml.xml_obj = data;
				callback(load_classes_xml.xml_obj);
			}});
		}else{
			callback(load_classes_xml.xml_obj);
		}
	};
	var url = '/js/classes.xml';
	option_init = '<option value="">请选择</option>';
	$.fn.setSubClassName = function(districtcode,target){
//		$.get(url,function(data) {
		load_classes_xml(function(data){
			var subclass_name = "",majorsort_name="";
			var majorsort_code=0,subclass_code=0;
			
			majorsort_code = Math.floor(districtcode/1000);
			subclass_code = districtcode;
			
			if(districtcode>10000)
			{
				majorsort_name = $('/*[code='+majorsort_code+']', $(data)).eq(0).attr("name");
			}
			if(districtcode>10000 && (districtcode % 10000 !=0))
			{
				subclass_name +=$('/*[code='+subclass_code+']', $(data)).eq(0).attr("name");
			}
			
			$(target).html(majorsort_name+' - '+subclass_name);
		});
	};

	$.fn.selectsort = function(majorsort,subclass,idcode){
		

		if(typeof(idcode)=="undefined"){
			subclass = 0;
		}
		var majorsort_code=0,subclass_code=0;
		
		majorsort_code = Math.floor(idcode/1000);
		subclass_code = idcode;
		
//	    $.get(url,function(data) {
		load_classes_xml(function(data){
	    	var setoptions = option_init;

	    	$('root>majorsort', data).each(function(){
				setoptions += '<option value="' + $(this).attr('code') +'"'
				if(majorsort_code==$(this).attr('code'))
					setoptions +=' selected="selected" ';				
				setoptions += '>' + $(this).attr('name') + '</option>';
				$(majorsort).html(setoptions);
		    });

			$(majorsort).change(function(){

				setoptions = option_init;
				if($('majorsort[code="'+$(this).val()+'"]', $(data)).children().length==0)
				{
					$(subclass).html('');
					$(subclass).hide();

				}else{
					$(subclass).show();
					$('majorsort[code="'+$(this).val()+'"]', $(data)).children().each(function(){
						setoptions += '<option value="' + $(this).attr('code') + '"'
						if(subclass_code==$(this).attr('code'))
							setoptions +=' selected="selected" ';						
						setoptions += '>' + $(this).attr('name') + '</option>';
						$(subclass).html(setoptions);
					});
					
					if($.browser.mozilla)
					{
						$(subclass).val(0);
						$(subclass).val(subclass_code);
					}
				}

			});	      

			if(majorsort_code>=10)
			{
				$(majorsort).show();			
				$(majorsort).trigger("change");
			}
			if(subclass_code>=10)
			{
				$(subclass).show();
			}
	    });
	};
})(jQuery);