"use strict";

// 取消提供源地址，解决防盗链问题 <https://www.jianshu.com/p/5b58ecce6443>
hexo.extend.injector.register("head_end", '<meta name="referrer" content="no-referrer" />');

// 自动调整markdown的图片地址
hexo.extend.filter.register("before_post_render", require("./autoAsset"));
// 根据一级标题自动设置名称
hexo.extend.filter.register("before_post_render", require("./autoTitle"));
// 根据路径自动设置分类
hexo.extend.filter.register("before_post_render", require("./autoCategories"));
