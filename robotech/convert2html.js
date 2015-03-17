
var modules=[];
//fill in the rest : 1 .. 9

modules[10] = {file:'RT100002.BOT', title:'robot safety'};
modules[11] = {file:'RT110002.BOT', title:'communications' };
modules[12] = {file:'RT120002.BOT', title:'applications' };
modules[13] = {file:'RT130002.BOT', title:'artificial intelligence' };

var module = 10;

if (process.argv.length >= 3) {
	module = process.argv[2];
}

var fs = require('fs');
var $ = require('cheerio');
var IN_DIR = '../ROBOTECH/';
var OUT_DIR = '';
var visited = [];
var pageNumber = 0;

var bigFile = $.load('<html><body></body></html>');
followFile(modules[module].file);

var sections = [];

var toc = createTOC();
//console.log(toc);
mainTOC = '<ul><li>' + toc.join('</li><li>') + '</li></ul>';

var navbar = '<p>' + toc.join('|') + '</p>';
bigFile('A').each(function(i, e) {
	var name = $(this).attr('name');
	if (sections.indexOf(name) > 0) {
		$(this).before(navbar);
	}
})

//console.log(mainTOC);
bigFile('BODY').prepend(mainTOC);

//toc GIF
bigFile('BODY').prepend('<img src="' + IN_DIR + 'GENERATE/mod' + module + '.gif">');
//obj GIF
bigFile('BODY').prepend('<img src="' + IN_DIR + 'GENERATE/obj' + module + '.gif">');

var outFileName = ('0' + module).substr(-2) + '-' + modules[module].title.replace(' ', '_') + '.html';
fs.writeFileSync(OUT_DIR + outFileName, bigFile.html());

function createTOC() {
	var toc = [];

	bigFile('SPAN').each(function(i, e) {
		var text = $(this).text().replace("\r\n", '');
		var style = $(this).attr('style');
		if (!style) {
			return true;
		}
		//console.log(text);
		if  ((style.search(/color/i) !== -1) && (text.search(/^[0-9]+-[0-9]+/i) !== -1) || 
			(text.search(/review *question/i) !== -1)) {
			//console.log(style, text);
			$(this).before('<a name="' + text + '"></a>');
			toc.push('<a href="#' + text +'">' + text + '</a>');
			sections.push(text);
		}
		if (text.search(/review *question/i) !== -1) {
			return false;
		}
	})

	return toc;
}

function appendReferencedFiles(file) {
	var links = false;
	file('A').each(function(i, e) {
		//console.log(this);
		//console.log($(this));
		var ref = getReferenceToAppend($(this));

		if (!ref.show) {
			return true;
		}
		
		var pos = ref.href.search(/\.bot/i);

		if (pos < 0) {
			return true;
		}

		//bigFile('BODY').append('<HR>');

		if (!links) {
			bigFile('BODY').append('<h3>Links</h3>');
			links = true;
		}

		if (ref.title) {
			bigFile('BODY').append('<h4>' + ref.title + '</h4>');
		}
		var file = $.load(fs.readFileSync(IN_DIR + ref.href));
		bigFile('BODY').append(file('BODY').html());
	});
}

function followFile(fileName) {
	if (visited.lastIndexOf(fileName) > -1) {
		return;
	}

	visited.push(fileName);

	var file = $.load(fs.readFileSync(IN_DIR + fileName));

	var hrefs = file('A', 'TABLE');
	var nextFile = "";
	if (hrefs.length === 3) {
		nextFile = file(hrefs[2]).attr('href');

		//We do not need the first table (the index)
		file('TABLE').first().remove();

		bigFile('BODY').append(file('BODY').html());

		appendReferencedFiles(file);

		bigFile('BODY').append('<p>Page ' + (++pageNumber) + '</p><hr>');

		//console.log(nextFile);

		followFile(nextFile);

	}

	//console.log(hrefs.length);
}

function getReferenceToAppend(hrefObj) {
	var href = hrefObj.attr('href');
	var retVal = {show: true, title: '', href: href};
	var html = hrefObj.html();

	if (href.search(/ShowPopup/) > -1) {
		retVal.href = href.replace("javascript:ShowPopup('",'').replace(/\'.*/, '');
	}

	//speaker icon in href
	if (html.search(/SPKR/) !== -1) {
		retVal.show = false;
	}

	//answer button in review questions, with showPopup
	if (html.search(/ANSWERSM/) !== -1) {
		retVal.show = true;
		retVal.title = 'Answer';
	}
	
	//reference button in review questions
	if (html.search(/REFERENC/) !== -1) {
		retVal.show = false;
	}

	//solution button in review questions
	if (html.search(/SOLUTSM/) !== -1) {
		return retVal;
	}

	return retVal;
}

function processFile(fileName, newFileName) {
	file = $.load(fs.readFileSync(fileName));
	

	file('A').each(function(i, e) {
		var href = $(this).attr('href'); 
		var pos = href.search(/RT.*\.bot/i);
		if (pos < 0) {
			return true;
		}
		$(this).attr('href', href.replace('.bot', '.html'));
		$(this).attr('target','_top');
		//console.log(i, $(this).text());
		if (!$(this).text() && ($(this).html().search(/ANSWERSM/) === -1) &&
			($(this).html().search(/REFERENC/) === -1) &&
			($(this).html().search(/SOLUTSM/) === -1)) {

			var fileName = $(this).attr('href').substr(pos);
			$(this).text(fileName);
		}
		
		if (href.search(/ShowPopup/) > -1) {
			//console.log(href);
			href = $(this).attr('href').replace("javascript:ShowPopup('",'').
				replace(/\'.*/, '');
			$(this).attr('href', href);
			
		}
		//console.log(i, $(this).text());	
	});

	fs.writeFileSync(newFileName, file.html());
}

function processSection(section) {
	var paddedSec = ("0000" + section).slice(-2);
	var re = new RegExp('RT' + paddedSec + '.*\.BOT');
	
	var files = fs.readdirSync('../ROBOTECH');

	for (var i = 0; i < files.length; i++) {
		var file = files[i];
		//console.log(file);
		var pos = files[i].search(re);
		if ( file.search(re) > -1) {
			var newFileName = file.substr(0, file.search(/\./)) + '.html';
			var fileName = '../ROBOTECH/' + file;
			processFile(fileName, newFileName);
		}
	}
}
