$("details").on("click", function() {
  $("details[open]")
      .not(this)
      .removeAttr("open");
});
