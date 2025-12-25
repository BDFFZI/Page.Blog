module.exports = function (data) {
  if (data.title !== null && data.title !== "") return;

  let titleExp = /^#\x20+(.+)/m;
  let title = data.content.match(titleExp);

  if (title === null) {
    console.warn(data.source + " Title is null");
    return;
  }

  data.title = title[1];
  // console.log(data.source + " Title found: " + title[1]);
};
