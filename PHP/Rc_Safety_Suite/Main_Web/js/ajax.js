/*
 * File:    ajax.js
 * Authors: Jesse Federkiewicz & Timothy Hachey
 * TLDR:    Powers AJAX Calls
*/

//////////////////////////////////////////////////////////////////////////////////////
// function AjaxRequest(url, type, data, dataType, successFunction, errorFunction)
// Args: 
//			url             : submit url
//          type            : GET OR POST
//          data            : data object
//          dataType        : json, html, etc.
//          successFunction : function to fire response data
//          errorFunction   : function to fire fail data
//
// NON-Query function
// will NOT expect a resultset to bind to result object
//
// Returns: rows effected
//////////////////////////////////////////////////////////////////////////////////////
function AjaxRequest(url, type, data, dataType, successFunction, errorFunction)
{
    let ajaxOptions = {};

    //Assign url
    ajaxOptions['url'] = url;

    //Let it know what type youre sending
    ajaxOptions['type'] = type  //GET OR POST

    //What data are you sending
    ajaxOptions['data'] = data;

    //What kind is the data youre sending
    ajaxOptions['dataType'] = dataType;

    //If success
    ajaxOptions['success'] = successFunction;

    //If error
    ajaxOptions['error'] = errorFunction;

    //actually submit the request
    return $.ajax(ajaxOptions);
}
