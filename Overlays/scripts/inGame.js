var showcorner, showlowerthird, showendgame, showcountdown, showI1, showI2, hasTlogo;
var updatingLT = false;
var showingG1 = false;
var showingG2 = false;
var updatingES = false;
var parser = new DOMParser();
var responseXml;
var fileUrl = "output/output.xml";
var refreshrate = 500;
var regex = /\{\{(\w{1,4})\}\}/g;
var match;

var trn, rd, s1, s2, t1, t2, gt, cdn, t1s, t2s, hr, snitch, streamer;
var g1, g2, endheight, endt1, endt2;

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
      g1 = $(xml).find('g1').text();
      g2 = $(xml).find('g2').text();
      endheight = $(xml).find('endheight').text();
      endt1 = $(xml).find('team1stats').text();
      endt2 = $(xml).find('team2stats').text();


      runUpdate();
    }
  });
}

function addIcons(text)
{
  text = text.replace(/\n/g, "<br>");
  match = regex.exec(text);

  while (match != null)
  {
    text = text.replace(match[0], "<span class=\"icon "+match[1]+"\"></span>");
    match = regex.exec(text);
  }

  return text;
}

function runUpdate() {
  if ( showcorner == "true" ) {
    $('#corner').removeClass('fast').addClass('display');

    // if a goal was scored by team 1, show the goal dropdown for 4s then remove
    if ( g1 && showingG1 == false )
    {
      showingG1 = true;

      $('#g1').html(addIcons(g1));
      $('#g1').parent().addClass('display');
      setTimeout( function() { $('#g1').parent().removeClass('display'); showingG1 = false; }, 5000);
    }

    // if a goal was scored by team 2, show the goal dropdown for 4s then remove
    if ( g2 && showingG2 == false )
    {
      showingG2 = true;

      $('#g2').html(addIcons(g2));
      $('#g2').parent().addClass('display');
      setTimeout( function() { $('#g2').parent().removeClass('display'); showingG2 = false; }, 5000);
    }
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
  $('.score1').text(s1);
  $('.score2').text(s2);
  $('#gt').text(gt);
  $('#cdn').text(cdn);
  $('#trn').text(trn);
  $('#rd').text(rd);
  $('#round').text(rd);
  $('#t1').text(t1);
  $('#t2').text(t2);

  if ( hr )
  {
    $('#hr').text("HR: " + hr);
  }

  if ( snitch )
  {
    $('#snitch').text("Snitch: " + snitch);
  }

  $('#streamer').html(addIcons(streamer));

  if ( showcountdown == "true" ) {
    $('#cdn').parent().addClass('display');
  }
  else {
    $('#cdn').parent().removeClass('display');
  }

  if ( showI1 == "true" ) {
    $('#i1c').addClass('thumb logo display ' + t1s);
    $('#i1l').addClass('thumb logo display ' + t1s);
    $('#i1e').addClass('thumb logo display ' + t1s);
  }
  else {
    $('#i1c').removeClass();
    $('#i1l').removeClass();
    $('#i1e').removeClass();
  }

  if ( showI2 == "true" ) {
    $('#i2c').addClass('thumb logo display ' + t2s);
    $('#i2l').addClass('thumb logo display ' + t2s);
    $('#i2e').addClass('thumb logo display ' + t2s);
  }
  else {
    $('#i2c').removeClass();
    $('#i2l').removeClass();
    $('#i2e').removeClass();
  }

  if ( hasTlogo == "true" )
  {
    $('.tourn').addClass('TN').removeClass('WW');
  }
  else
  {
    $('.tourn').removeClass('TN').addClass('WW');
  }

  if ( showlowerthird == "true" && updatingLT == false)
  {
    updatingLT = true;
    $('#tournl').addClass('display')
    setTimeout( function() { $('.sidebar').addClass('display'); }, 1000);
    setTimeout( function() { $('.streamer').addClass('display'); updatingLT = false; }, 2000);
  }
  else if ( updatingLT == false )
  {
    updatingLT = true;
    $('.streamer').removeClass('display');
    setTimeout( function() { $('.sidebar').removeClass('display'); }, 1000);
    setTimeout( function() { $('#tournl').removeClass('display'); updatingLT = false; }, 2000);
  }

  if ( showendgame == "true" && updatingES == false)
  {
    updatingES = true;
    $('#t1stats').html(addIcons(endt1));
    $('#t2stats').html(addIcons(endt2));
    $('#endscreen').get(0).style.setProperty("--stat-height", endheight);
    $('#tourne').addClass('display')
    setTimeout( function() { $('.endsidebar').addClass('display'); }, 1000);
    setTimeout( function() { $('.endround').addClass('display'); }, 2000);
    setTimeout( function() { $('#stats').addClass('display'); updatingES = false; }, 3000);
  }
  else if ( updatingES == false )
  {
    updatingES = true;
    $('.endround').removeClass('display');
    setTimeout( function() { $('.endsidebar').removeClass('display'); }, 1000);
    setTimeout( function() { $('#tourne').removeClass('display'); }, 2000);
    setTimeout( function() { $('#stats').removeClass('display'); updatingES = false; }, 3000);
  }

}
