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
