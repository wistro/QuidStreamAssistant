var showcorner, showlowerthird, showendgame, showcountdown, showI1, showI2, hasTlogo;
var updating = false;
var parser = new DOMParser();
var responseXml;
var fileUrl = "output/output.xml";
var refreshrate = 500;

var trn, rd, s1, s2, t1, t2, gt, cdn, t1s, t2s, hr, snitch, streamer;

$(function() {
	checkUpdate();
	setInterval(function() { checkUpdate(); }, refreshrate);
});

function checkUpdate() {
	//if (updating) return;
	getRequest();
}

function getRequest() {
	$.ajax({
    url: fileUrl,
    dataType: "xml",
    ifModified: true,
    success: function(xml) {
      showcorner = $(xml).find('corner').text();
      showlowerthird = $(xml).find('lowerthird').text();
      showendgame = $(xml).find('endscreen').text();
      showcountdown = $(xml).find('sop').text();
      trn = $(xml).find('tournament').text();
      rd = $(xml).find('round').text();
      s1 = $(xml).find('s1').text();
      s2 = $(xml).find('s2').text();
      t1 = $(xml).find('t1').text();
      t2 = $(xml).find('t2').text();
      t1s = $(xml).find('t1short').text();
      t2s = $(xml).find('t2short').text();
      gt = $(xml).find('gt').text();
      cdn = $(xml).find('countdown').text();
      showI1 = $(xml).find('t1logo').text();
      showI2 = $(xml).find('t2logo').text();
      hasTlogo = $(xml).find('tournlogo').text();
      hr = $(xml).find('hr').text();
      snitch = $(xml).find('snitch').text();
      streamer = $(xml).find('streamer').text();

      runUpdate();
    }
  });
}

function runUpdate() {
  if ( showcorner == "true" ) {
    $('#corner').removeClass('fast').addClass('display');
  }
  else {
    $('#corner').removeClass('display').addClass('fast');
  }

  if ( s1.length > 5 ) {
    $('#s1size').removeClass('small').addClass('xsmall');
  }
  else {
    $('#s1size').removeClass('xsmall').addClass('small');
  }

  if ( s2.length > 5 ) {
    $('#s2size').removeClass('small').addClass('xsmall');
  }
  else {
    $('#s2size').removeClass('xsmall').addClass('small');
  }

	$('#t1s').text(t1s);
  $('#t2s').text(t2s);
  $('#s1').text(s1);
  $('#s2').text(s2);
  $('#gt').text(gt);
  $('#cdn').text(cdn);
  $('#trn').text(trn);
  $('#rd').text(rd);

  if ( hr )
  {
    $('#hr').text("HR: " + hr);
  }

  if ( snitch )
  {
    $('#snitch').text("Snitch: " + snitch);
  }
  
  $('#streamer').text(streamer);
  streamer = streamer.replace('iTW', '<span class="icon iTW"></span>');


  if ( showcountdown == "true" ) {
    $('#cdn').parent().addClass('display');
  }
  else {
    $('#cdn').parent().removeClass('display');
  }

  if ( showI1 == "true" ) {
    $('#i1').addClass('thumb logo display ' + t1s);
  }
  else {
    $('#i1').removeClass();
  }

  if ( showI2 == "true" ) {
    $('#i2').addClass('thumb logo display ' + t2s);
  }
  else {
    $('#i2').removeClass();
  }

  if ( hasTlogo == "true" )
  {
    $('#tourn').addClass('TN').removeClass('WW');
  }
  else
  {
    $('#tourn').removeClass('TN').addClass('WW');
  }

  if ( showlowerthird == "true" && updating == false)
  {
    updating = true;
    $('#tourn').addClass('display')
    setTimeout( function() { $('.sidebar').addClass('display'); /*$('#tourn').addClass('display');*/ }, 2000);
    setTimeout( function() { $('.streamer').addClass('display'); updating = false; }, 4000);
  }
  else if ( updating == false )
  {
    updating = true;
    $('.streamer').removeClass('display');
    setTimeout( function() { $('.sidebar').removeClass('display'); /*$('#tourn').removeClass('display');*/ }, 2000);
    setTimeout( function() { $('#tourn').removeClass('display'); updating = false; }, 4000);
  }

}
