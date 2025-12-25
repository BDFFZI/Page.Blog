const fs = require('fs');
const path = require('path');

const imageRoot = "/assets/images";

module.exports = function (data) {
  //替换相对图片
  data.content = data.content.replace(/\!\[(.*)\]\(<?(.*)>?\)/g, (match, imageName, imageUrl, offset, string) => {
    //未使用相对路径，直接返回
    if (imageUrl[0] !== ".") return match;

    // console.log(data.source + " >> Replace ImageUrl: " + imageUrl);

    //使用相对路径，替换为图床目录
    const fileName = imageUrl.replace(/.*\/(.*)/, "$1");
    return `![${imageName}](${imageRoot}/${fileName})`;
  });

  //替换相对文章
  data.content = data.content.replace(/[^!]\[(.*)\]\(<?(.*)>?\)/g, (match, postName, postUrl, offset, string) => {
    //未使用相对路径，直接返回
    if (postUrl[0] !== ".") return match;

    // console.log(data.source + " >> Replace PostUrl: " + postUrl);

    //获取引用文件路径
    const currentDir = path.dirname(data.full_source);
    const targetPath = path.resolve(currentDir, postUrl);
    //读取文章的永久链接
    const targetContent = fs.readFileSync(targetPath, 'utf-8');
    const targetAbbrlink = targetContent.match(/abbrlink:\s*(\S+)/)[1];

    //使用永久链接
    return `[${postName}](/posts/${targetAbbrlink}/)`;
  });

  return data;
};
