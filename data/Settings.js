var password = document.getElementById("password")
  , confirm_password = document.getElementById("confirm_password");

var alertRedInput = "#8C1010";
var defaultInput = "rgba(10, 180, 180, 1)";
   
function passwordValidation(passwordInput) {
    var password = document.getElementById("password");
    var issueArr = [];
    if (!/^.{7,15}$/.test(passwordInput)) {
        issueArr.push("Password must be between 7-15 characters.");
    }
    if (!/\d/.test(passwordInput)) {
        issueArr.push("Must contain at least one number.");
    }
    if (!/[a-z]/.test(passwordInput)) {
        issueArr.push("Must contain a lowercase letter.");
    }
    if (!/[A-Z]/.test(passwordInput)) {
        issueArr.push("Must contain an uppercase letter.");
    }
    if (issueArr.length > 0) {
        password.setCustomValidity(issueArr.join("\n"));
        password.style.borderColor = alertRedInput;
    } else {
        password.setCustomValidity("");
        password.style.borderColor = defaultInput;
    }
}

function validatePassword(){
    var password = document.getElementById("password");
    if(password.value != confirm_password.value) {
        confirm_password.setCustomValidity("Passwords Don't Match");
        confirm_password.style.borderColor = alertRedInput;
    }
    else 
    {
        confirm_password.setCustomValidity('');
        confirm_password.style.borderColor = defaultInput;
    }
}

password.onchange = validatePassword;
confirm_password.onkeyup = validatePassword;