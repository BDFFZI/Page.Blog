const front = require("hexo-front-matter");
const fs = require("hexo-fs");

module.exports = function (data) {
  // 获取路径信息
  const split = data.source.split("/");
  const depth = split.length - 2;
  if (depth === 0)
    //没有进行分类
    return;

  // 计算出分类
  const categories = split.slice(1, 1 + depth);

  //读取原始文件，判断分类是否相同
  const sourcePost = front.parse(data.raw);
  if (sourcePost.categories !== undefined && sourcePost.categories.join("_") === categories.join("_")) return;

  //将带有分类信息的文本写回到源文件
  sourcePost.categories = categories;
  let postStr = "---\n" + front.stringify(sourcePost, { mode: "yaml" });
  fs.writeFile(data.full_source, postStr, "utf-8");
  console.log("Generated: categories [%s] for post [%s]", sourcePost.categories, categories[categories.length - 1]);
};
