var lastPage = sessionStorage.getItem("lastPage");
if (window.location.href !== lastPage) {
  let random = Math.random().toString(36).substring(2);
  let newUrl = window.location.href + "?random=" + random;

  sessionStorage.setItem("lastPage", newUrl);
  window.location.href = newUrl;
}
