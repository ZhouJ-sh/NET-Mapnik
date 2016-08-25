using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Collections.Generic;

namespace NETMapnik.Test
{
    [TestClass]
    public class StyleTest
    {
        [TestMethod]
        public void Style_Get()
        {
            var m = new Map(256, 256);
            m.Load(@".\data\style.xml");
            var styles = m.Styles() as Dictionary<string, FeatureTypeStyle>;
            Assert.AreEqual(1, styles.Count);
            Assert.AreEqual(true, styles.ContainsKey("style"));

            var style = m.GetStyle("style");
            Assert.IsNotNull(style);
        }

        [TestMethod]
        public void Style_GetUnicode()
        {
            var m = new Map(256, 256);
            m.Load(@".\data\style_unicode.xml");
            var styles = m.Styles() as Dictionary<string, FeatureTypeStyle>;
            Assert.AreEqual(1, styles.Count);
            Assert.AreEqual(true, styles.ContainsKey("样式"));

            var style = m.GetStyle("样式");
            Assert.IsNotNull(style);
        }

        [TestMethod]
        public void Style_Add()
        {
            var m = new Map(256, 256);
            var style = new FeatureTypeStyle();
            m.AddStyle("style", style);
            m.AddStyle("样式", style);
            var styles = m.Styles() as Dictionary<string, FeatureTypeStyle>;

            Assert.AreEqual(2, styles.Count);
            Assert.AreEqual(true, styles.ContainsKey("样式"));
            Assert.AreEqual(true, styles.ContainsKey("style"));
        }
    }
}
