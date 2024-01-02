const validUser = document.getElementById('userStatus').innerHTML;

if(!validUser){
  document.getElementById("logoutLink").style.display = none;
}
else{
  document.getElementById("loginLink").style.display = none;
}