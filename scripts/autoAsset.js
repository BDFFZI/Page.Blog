const imageRoot = "/assets/images";

module.exports = function (data) {
  data.content = data.content.replace(/\!\[(.*)\]\(<?(.*)>?\)/g, (match, imageName, imageUrl, offset, string) => {
    //未使用相对路径，直接返回
    if (imageUrl[0] !== ".") return match;

    //使用相对路径，替换为图床目录
    const fileName = imageUrl.replace(/.*\/(.*)/, "$1");
    return `![${imageName}](${imageRoot}/${fileName})`;
  });
  return data;
};
